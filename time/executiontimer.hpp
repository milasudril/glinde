//@	{
//@	 "targets":[{"name":"executiontimer.hpp","type":"include"}]
//@	}

#ifndef GLINDE_EXECUTIONTIMER_HPP
#define GLINDE_EXECUTIONTIMER_HPP

#ifdef _WIN32
#error "Not implemented"
#else
#include <time.h>

namespace Glinde
	{
	class ExecutionTimer
		{
		public:
			ExecutionTimer() noexcept
				{reset();}

			double value() const noexcept
				{
				timespec now;
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&now);
				auto ret=static_cast<double>(now.tv_sec - m_start.tv_sec);
				return ret + (now.tv_nsec - m_start.tv_nsec)/1e9;
				}

			void reset() noexcept
				{clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&m_start);}

		private:
			timespec m_start;
		};
	}

#endif
#endif
