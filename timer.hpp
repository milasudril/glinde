//@	{
//@	"targets":[{"name":"timer.hpp","type":"include"}]
//@	}

#ifndef GLINDE_TIMER_HPP
#define GLINDE_TIMER_HPP

namespace Glinde
	{
	class Timer
		{
		public:
			virtual ~Timer()=default;
			virtual void wait() const noexcept=0;
			virtual double delay() const noexcept=0;
		};
	}

#endif
