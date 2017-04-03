//@	{"targets":[{"name":"logdefault.o","type":"object"}]}


#if __MINGW32__ || __MINGW64__
	#define __USE_MINGW_ANSI_STDIO 1
#endif

#include "logdefault.hpp"
#include "../stringformat.hpp"
#include "../variant.hpp"
#include "../messagequeue/messagequeue.hpp"
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
			void progress(const char* message,double x) noexcept
				{
				auto val=static_cast<int>(100*x);
				fprintf(stderr,"[%d%%] Extracting %s\n",val,message);
				}

			void progressInit() noexcept
				{}

			void progressEnd() noexcept
				{}
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

void LogDefault::write(MessageType type,const char* message) noexcept
	{
	auto k=static_cast<unsigned int>( m_writers.length() );
	while(k!=0)
		{
		--k;
		if(m_writers[k]!=nullptr)
			{m_writers[k]->write(type,message);}
		}
	}

void LogDefault::operator()(const Timeinfo& ti,const String& str,Log::MessageType type) noexcept
	{write(type,str.begin());}


void LogDefault::write(Log::MessageType type,const char* format_string
	,const std::initializer_list<Variant>& args)
	{
	char msgbuff[4096];
	format(Range<char>(msgbuff,sizeof(msgbuff)/sizeof(msgbuff[0])),format_string
		,args);
	if(r_queue==nullptr)
		{write(type,msgbuff);}
	else
		{r_queue->post(0,Message{*this,String(msgbuff),std::move(type)});}
	}


void LogDefault::progressInit()
	{
	if(r_queue==nullptr)
		{progress(ProgressInit{});}
	else
		{r_queue->post(0,Message{*this,ProgressInit{},0});}
	}

void LogDefault::progressEnd()
	{
	if(r_queue==nullptr)
		{progress(ProgressEnd{});}
	else
		{r_queue->post(0,Message{*this,ProgressEnd{},0});}
	}


void LogDefault::progress(ProgressInit) noexcept
	{
	auto k=static_cast<unsigned int>( m_writers.length() );
	while(k!=0)
		{
		--k;
		if(m_writers[k]!=nullptr)
			{m_writers[k]->progressInit();}
		}
	}

void LogDefault::progress(ProgressEnd) noexcept
	{
	auto k=static_cast<unsigned int>( m_writers.length() );
	while(k!=0)
		{
		--k;
		if(m_writers[k]!=nullptr)
			{m_writers[k]->progressEnd();}
		}
	}

void LogDefault::progress(double x,const char* message)
	{
	if(r_queue==nullptr)
		{progress(message,x);}
	else
		{r_queue->post(0,Message{*this,String(message),std::move(x)});}
	}

void LogDefault::progress(const char* message,double x) noexcept
	{
	auto k=static_cast<unsigned int>( m_writers.length() );
	while(k!=0)
		{
		--k;
		if(m_writers[k]!=nullptr)
			{m_writers[k]->progress(message,x);}
		}
	}

void LogDefault::operator()(const Timeinfo& ti,ProgressInit i,int) noexcept
	{progress(i);}

void LogDefault::operator()(const Timeinfo& ti,ProgressEnd i,int) noexcept
	{progress(i);}

void LogDefault::operator()(const Timeinfo& ti,const String& str,double x) noexcept
	{progress(str.begin(),x);}


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
		,"(x) The evil boss has setup a deathtrap because of #0; "
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
