#ifdef __WAND__
target[name[logwriter.o] type [object]]
#endif

#include "logwriter.h"

#if __MINGW32__ || __MINGW64__
	#define __USE_MINGW_ANSI_STDIO 1
#endif

#include <cstdio>
#include <cstdarg>

using namespace Glinde;

namespace
	{
	class LogWriterStderr:public LogWriter
		{
		public:
			void write(LogMessageType type, const char* message);
		};
	}

void LogWriterStderr::write(LogMessageType type,const char* message)
	{
	const char* infostring;
	switch(type)
		{
		case LogMessageType::INFORMATION:
			infostring="(i)";
			break;

		case LogMessageType::WARNING:
			infostring="(!)";
			break;

		case LogMessageType::ERROR:
			infostring="(x)";
			break;

		case LogMessageType::DEBUG:
			infostring="(DEBUG)";
			break;

		case LogMessageType::LINE_QUOTE:
			infostring=" ";
			break;
		}
	fprintf(stderr,"%s %s\n",infostring,message);
	fflush(stderr);
	}

static LogWriterStderr s_stderr;
static LogWriter* s_writer=nullptr;

void Glinde::logWrite(LogMessageType type,const char* format,...)
	{
	char msgbuff[4096];
	va_list list;
	va_start(list,format);
	vsnprintf(msgbuff,4096,format,list);
	va_end(list);
	msgbuff[4095]=0;

	s_stderr.write(type,msgbuff);
	if(s_writer!=nullptr)
		{
		s_writer->write(type,msgbuff);
		}
	}

void Glinde::logWriterAttach(LogWriter* writer)
	{
	s_writer=writer;
	}


#if (_WIN32 || _WIN64)
void Glinde::deathtrapHandlerActivate()
	{}
#else

#include <signal.h>
#include <unistd.h>

inline static const void* faultAddressGet(ucontext_t* context)
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

	auto n=snprintf(buffer,1024
		,"(x) The wicked Witch of the East has setup a deathtrap because of %s "
		"at %p. The program will now terminate.\n",reason,ptr);
	write(STDERR_FILENO,buffer,n);
	_exit(signal);
	}

void Glinde::deathtrapHandlerActivate()
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
