//@	{
//@	 "targets":
//@		[{
//@		 "dependencies":[{"ref":"rt","rel":"external"},{"ref":"pthread","rel":"external"}]
//@		,"name":"timerreal.o","type":"object"
//@		}]
//@	}

#include "timerreal.hpp"
#include "../errormessage.hpp"
#include "../variant.hpp"
#include <signal.h>
#include <time.h>
#include <semaphore.h>

using namespace Glinde;

struct TimerReal::Impl
	{
	Impl(double frequency)
		{
		if(sem_init(&m_trig,0,1)!=0)
			{throw ErrorMessage("It was not possible to initialize a semaphore for the timerreal",{});}

		sigevent ev;

		ev.sigev_notify=SIGEV_SIGNAL;
		ev.sigev_signo=SIGRTMIN;
		ev.sigev_value.sival_ptr=&m_trig;

		struct sigaction sa;
		sa.sa_flags=SA_SIGINFO;
		sa.sa_sigaction=sig_handler;
		sigemptyset(&sa.sa_mask);

		if(sigaction(SIGRTMIN,&sa,NULL)!=0)
			{
			sem_destroy(&m_trig);
			throw ErrorMessage("It was not possible to register a new signal handler for SIGRTMIN.",{});
			}
		if(timer_create(CLOCK_MONOTONIC,&ev,&m_id)!=0)
			{
			sem_destroy(&m_trig);
			throw ErrorMessage("It was not possible to create a new timerreal.",{});
			}

		itimerspec t;
		auto T=1/frequency;
		auto delay_sec=static_cast<time_t>(T);
		t.it_interval=
			{
			 delay_sec
			,static_cast<long>(1e9*(T-static_cast<double>(delay_sec)) + 0.5)
			};
		t.it_value={0,1};
		if(timer_settime(m_id,0,&t,NULL)!=0)
			{
			throw ErrorMessage("It was not possible to set the timerreal interval.",{});
			timer_delete(m_id);
			sem_destroy(&m_trig);
			}
		}

	~Impl()
		{
		itimerspec t={{0,0},{0,0}};
		timer_settime(m_id,0,&t,NULL);
		timer_delete(m_id);
		sem_destroy(&m_trig);
		}

	void wait() noexcept
		{sem_wait(&m_trig);}

	sem_t m_trig;
	timer_t m_id;

	static void sig_handler(int signal,siginfo_t* si,void* uc)
		{
		sem_post( reinterpret_cast<sem_t*>(uc) );
		}

	double delay() const noexcept
		{
		itimerspec t;
		timer_gettime(m_id,&t);

		return static_cast<double>(t.it_interval.tv_sec)
			+ 1e-9*static_cast<double>(t.it_interval.tv_nsec);
		}
	};

TimerReal::TimerReal(double frequency)
	{
	m_impl=new Impl(frequency);
	}

TimerReal::~TimerReal()
	{
	delete m_impl;
	}

void TimerReal::wait() noexcept
	{m_impl->wait();}

double TimerReal::delay() const noexcept
	{return m_impl->delay();}
