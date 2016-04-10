#ifdef __WAND__
target[name[timer.o] type[object] platform[;GNU/Linux] dependency[rt;external]]
#endif


#include "timer.h"
#include "errormessage.h"
#include "debug.h"
#include <signal.h>
#include <time.h>


using namespace Glinda;

struct Timer::Impl
	{
	Impl(double frequency)
		{
		GLINDA_DEBUG_PRINT("Creating a timer %p",this);
		sigevent ev;

		ev.sigev_notify=SIGEV_SIGNAL;
		ev.sigev_signo=SIGRTMIN;
		ev.sigev_value.sival_ptr=this;

		struct sigaction sa;
		sa.sa_flags=SA_SIGINFO;
		sa.sa_sigaction=sig_handler;
		sigemptyset(&sa.sa_mask);

		if(sigaction(SIGRTMIN,&sa,NULL)!=0)
			{throw ErrorMessage("It was not possible to register a new signal handler for SIGRTMIN.");}
		if(timer_create(CLOCK_MONOTONIC,&ev,&m_id)!=0)
			{throw ErrorMessage("It was not possible to create a new timer.");}

		itimerspec t;
		auto T=1/frequency;
		auto delay_sec=static_cast<time_t>(T);
		t.it_interval=
			{
			 delay_sec
			,static_cast<long>(1e9*(T-static_cast<double>(delay_sec)))
			};
		t.it_value={0,1};
		if(timer_settime(m_id,0,&t,NULL)!=0)
			{
			throw ErrorMessage("It was not possible to set the timer interval.");
			timer_delete(m_id);
			}
		}

	~Impl()
		{
		itimerspec t={{0,0},{0,0}};
		timer_settime(m_id,0,&t,NULL);
		timer_delete(m_id);
		}

	timer_t m_id;

	static void sig_handler(int signal,siginfo_t* si,void* uc)
		{
		GLINDA_DEBUG_PRINT("Hello %p",si->si_ptr);
		}
	};

Timer::Timer(double frequency)
	{
	m_impl=new Impl(frequency);
	}

Timer::~Timer()
	{
	delete m_impl;
	}

void Timer::wait() noexcept
	{}
