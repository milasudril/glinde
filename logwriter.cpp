#ifdef __WAND__
target[name[logwriter.o] type [object]]
#endif

#include "logwriter.h"

#include <cstdio>
#include <cstdarg>

using namespace Glinie;

void Glinie::logWrite(LogMessageType type,const char* format,...)
	{
	const char* infostring;
	switch(type)
		{
		case LogMessageType::INFORMATION:
			infostring="i";
			break;

		case LogMessageType::WARNING:
			infostring="!";
			break;

		case LogMessageType::ERROR:
			infostring="x";
			break;

#ifndef NDEBUG
		case LogMessageType::DEBUG:
			infostring="DEBUG";
			break;
#endif
		}
	fprintf(stderr,"(%s) ",infostring);
	va_list list;
	va_start(list,format);
	vfprintf(stderr,format,list);
	va_end(list);
	putc('\n',stderr);
	}


#if (_WIN32 || _WIN64)
void Glinie::deathtrapHandlerActivate()
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

void Glinie::deathtrapHandlerActivate()
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
