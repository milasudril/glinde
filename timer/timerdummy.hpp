//@	{"targets":[{"name":"timerdummy.hpp","type":"include"}]}

#ifndef GLINDE_TIMEDUMMY_HPP
#define GLINDE_TIMEDUMMY_HPP

#include "timer.hpp"

namespace Glinde
	{
	class TimerDummy:public Timer
		{
		public:
			explicit TimerDummy(double claimed_frequency):m_delay(1/claimed_frequency)
				{}

			/**\brief Blocks the calling thread until the timer has made a tick.
			 *
			 * This function blocks the calling thread until the timer has made
			 * a tick.
			 *
			 * \note This kind of Timer only simulates the behaviour of a real
			 * timer, without any actual blocking.
			*/
			void wait() noexcept
				{}

			/**\brief Gets the time interval in seconds between two frames.
			 *
			 * This function returns the number of seconds between two frames.
			*/
			double delay() const noexcept
				{return m_delay;}


		private:
			double m_delay;
		};
	}

#endif
