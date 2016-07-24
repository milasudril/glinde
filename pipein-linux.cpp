//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"pipein.o",
//@	            "type":"object"
//@	        }
//@	    ]
//@	}
#include "pipein.h"
#include "errormessage.h"
#include "arraysimple.h"
#include "strerror.h"
#include "variant.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>


using namespace Glinde;

static constexpr int READ_FD=0;
static constexpr int WRITE_FD=1;

struct Glinde::PipeIn::Impl
	{
	int fd;
	int pid;
	int status;
	};

PipeIn::PipeIn(Stream stream_capture,const char* command
	,const Range<const char* const>& args):m_impl(new Impl)
	{
	int child_to_parent[2];
	int child_execfailed[2];

	if(pipe(child_to_parent)==-1)
		{
		char errbuff[256];
		strerror(errno,errbuff,256);
		throw ErrorMessage("It was not possible to create an IPC pipe. #0;",{errbuff});
		}

	if(pipe(child_execfailed)==-1)
		{
		auto status=errno;
		close(child_to_parent[0]);
		close(child_to_parent[1]);
		char errbuff[256];
		strerror(status,errbuff,256);
		throw ErrorMessage("It was not possible to create an IPC pipe. #0;",{errbuff});
		}

//	Fill argument array
	ArraySimple<const char*> argv(args.length() + 2);
	auto pos=argv.begin();
	*pos=command;
	ArrayInit::copy(pos + 1,argv.end() - 1,args.begin());
	*(argv.end() - 1)=nullptr;

	auto child=fork();
	if(child==-1)
		{
		auto status=errno;
		close(child_to_parent[0]);
		close(child_to_parent[1]);
		close(child_execfailed[0]);
		close(child_execfailed[1]);
		char errbuff[256];
		strerror(status,errbuff,256);
		throw ErrorMessage("It was not possible to create a child process. #0;",{errbuff});
		}

	if(child==0)
		{
	//	In child process
		close(child_execfailed[READ_FD]);
		fcntl(child_execfailed[WRITE_FD],FD_CLOEXEC);
		close(child_to_parent[READ_FD]);
		dup2(child_to_parent[WRITE_FD]
			,stream_capture==Stream::STDERR?STDERR_FILENO:STDOUT_FILENO);
	//	const char *envp[] = {"LC_ALL=\"C\"",nullptr};
		if(execvp(argv[0],const_cast<char* const*>(argv.begin()))==-1)
			{
			int status=errno;
			write(child_execfailed[WRITE_FD],&status,sizeof(status));
			_exit(status); //Exit closes remaining file descriptors on this process
			}
		}

//	In parent process
	close(child_execfailed[WRITE_FD]);
	close(child_to_parent[WRITE_FD]);

	int status=0;
//	Check if execvp failed.
	if(::read(child_execfailed[READ_FD],&status,sizeof(status))==sizeof(status))
		{
		close(child_execfailed[READ_FD]);
		close(child_to_parent[READ_FD]);
		char errbuff[256];
		strerror(status,errbuff,256);
		waitpid(child,&status,0);
		throw ErrorMessage("It was not possible to start #0;. #0;1"
			,{argv[0],errbuff});
		}

//	Everything is OK. Ready to go
	close(child_execfailed[READ_FD]);

	m_impl->fd=child_to_parent[READ_FD];
	m_impl->pid=child;
	m_impl->status=0;
	}

PipeIn::~PipeIn()
	{
	close(m_impl->fd);
	exitStatusGet();
	}

int PipeIn::exitStatusGet() noexcept
	{
	if(m_impl->pid!=0)
		{
		waitpid(m_impl->status,&m_impl->status,0);
		m_impl->pid=0;
		}
	return WEXITSTATUS(m_impl->status);
	}

size_t PipeIn::read(void* buffer,size_t count)
	{
	auto pos=reinterpret_cast<uint8_t*>(buffer);
	size_t n_read=0;
	while(n_read!=count)
		{
		auto n=::read(m_impl->fd,pos,count-n_read);
		if(n==0)
			{return n_read;}
		if(n==-1)
			{throw ErrorMessage("I/O error",{});}
		pos+=n;
		n_read+=n;
		}
	return n_read;
	}
