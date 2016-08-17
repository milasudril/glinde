//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"logdefault.o",
//@	            "type":"object"
//@	        }
//@	    ]
//@	}

#if __MINGW32__ || __MINGW64__
	#define __USE_MINGW_ANSI_STDIO 1
#endif


#include "logdefault.h"
#include "stringformat.h"
#include "variant.h"
#include "messagequeue.h"
#include <cstdio>
#include <cstdarg>
#include <cstring>

using namespace Glinde;

namespace
	{
	class LogWriterStderr:public Log::Writer
		{
		public:
			void write(Log::MessageType type, const char* message) noexcept;
		};
	}

void LogWriterStderr::write(Log::MessageType type,const char* message) noexcept
	{
	const char* infostring;
	switch(type)
		{
		case Log::MessageType::INFORMATION:
			infostring="(i)";
			break;

		case Log::MessageType::WARNING:
			infostring="(!)";
			break;

		case Log::MessageType::ERROR:
			infostring="(x)";
			break;

		case Log::MessageType::DEBUG:
			infostring="(DEBUG)";
			break;

		case Log::MessageType::LINE_QUOTE:
			infostring=" ";
			break;
		}
	fprintf(stderr,"%s %s\n",infostring,message);
	fflush(stderr);
	}

static LogWriterStderr s_writer_stderr;

LogDefault::LogDefault() noexcept:r_queue(nullptr)
	{
	memset(m_writers.begin(),0,sizeof(m_writers));
	writerAttach(s_writer_stderr);
	}

void LogDefault::write(MessageType type,const char* message)
	{
	auto k=static_cast<unsigned int>( m_writers.length() );
	while(k!=0)
		{
		--k;
		if(m_writers[k]!=nullptr)
			{m_writers[k]->write(type,message);}
		}
	}

void LogDefault::operator()(const Message& message)
	{
	auto data=message.dataGet<char>();
	auto type=static_cast<MessageType>( message.paramsGet<uint32_t>()[1] );
	write(type,data.begin());
	}

void LogDefault::write(Log::MessageType type,const char* format_string
	,const std::initializer_list<Variant>& args) noexcept
	{
	char msgbuff[4096];
	format(Range<char>(msgbuff,sizeof(msgbuff)/sizeof(msgbuff[0])),format_string
		,args);
	if(r_queue==nullptr)
		{write(type,msgbuff);}
	else
		{
		auto r=Range<const char>(msgbuff,strlen(msgbuff) + 1);
		ArrayFixed<uint32_t,1> params{static_cast<uint32_t>( type )};
		r_queue->post(Message{0,*this,r,params});
		}
	}

unsigned int LogDefault::writerAttach(Log::Writer& writer) noexcept
	{
	auto k=static_cast<unsigned int>( m_writers.length() );
	while(k!=0)
		{
		--k;
		if(m_writers[k]==nullptr)
			{
			m_writers[k]=&writer;
			return k;
			}
		}
	return static_cast<unsigned int>(-1);
	}

void LogDefault::writerDetach(unsigned int index) noexcept
	{
	if(index!=static_cast<unsigned int>(-1))
		{m_writers[index]=nullptr;}
	}



///Deathtrap handlers

#if (_WIN32 || _WIN64)
void Glinde::deathtrapHandlerActivate()
	{
//	TODO
	}
#else

#include <signal.h>
#include <unistd.h>

inline static const void* faultAddressGet(ucontext_t* context) noexcept
	{
#if (__amd64 || __x86_64 || __x86_64__ || __amd64__)
	return reinterpret_cast<void*>(context->uc_mcontext.gregs[REG_RIP]);
#else
	return reinterpret_cast<void*>(context->uc_mcontext.gregs[REG_EIP]);
#endif
	}

[[noreturn]] static void deathMessagePrint(int signal,siginfo_t* info, void* context)
	{
	char buffer[1024];
	const char* reason="an unkown error";

	switch(signal)
		{
		case SIGILL:
			reason="an illegal instruction";
			break;

		case SIGSEGV:
			reason="a segmentation fault";
			break;

		case SIGFPE:
			reason="a floating point exception";
			break;

		case SIGBUS:
			reason="a bus error";
			break;

		case SIGTRAP:
			reason="a trap";
			break;
		}

	auto ptr=faultAddressGet((ucontext_t*)context);

	format(Range<char>(buffer,sizeof(buffer)/sizeof(buffer[0]))
		,"(x) The wicked Witch of the East has setup a deathtrap because of #0; "
		"at #1;. The program will now terminate.\n",{reason,ptr});
	write(STDERR_FILENO,buffer,strlen(buffer));
	_exit(signal);
	}

void LogDefault::deathtrapHandlerActivate()
	{
	struct sigaction sa;
	sa.sa_sigaction=deathMessagePrint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags=SA_SIGINFO;
	sa.sa_restorer=nullptr;

	struct sigaction sa_old;

	sigaction(SIGILL,&sa,&sa_old);
	sigaction(SIGFPE,&sa,&sa_old);
	sigaction(SIGSEGV,&sa,&sa_old);
	sigaction(SIGBUS,&sa,&sa_old);
	sigaction(SIGTRAP,&sa,&sa_old);
	}
#endif
