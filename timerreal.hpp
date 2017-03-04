//@	{
//@	 "dependencies_extra":[{"ref":"timerreal.o","rel":"implementation"}]
//@	,"targets":[{"name":"timerreal.h","type":"include"}]
//@	}

/**\file timerreal.h \brief Defines a periodic timer.
 */

#ifndef GLINDE_TIMERREAL_HPP
#define GLINDE_TIMERREAL_HPP

#include "timer.hpp"


namespace Glinde
	{
	/**\brief A periodic Timer that ticks at predefined frequency.
	 *
	 */
	class TimerReal:public Timer
		{
		public:
			/**\brief Initializes the timer so it ticks with the given
			 * frequency.
			 *
			 * This constructor initializes the timer so it ticks with the given
			 * frequency.
			 *
			 * \param frequency The event frequency given in Hz.
			 *
			*/
			explicit TimerReal(double frequency);
			~TimerReal();

			/**\brief Blocks the calling thread until the timer has made a tick.
			 *
			 * This function blocks the calling thread until the timer has made
			 * a tick.
			 *
			*/
			void wait() noexcept;

			/**\brief Gets the time interval in seconds between two frames.
			 *
			 * This function returns the number of seconds between two frames.
			*/
			double delay() const noexcept;


		private:
			TimerReal(const TimerReal&)=delete;
			TimerReal& operator=(const TimerReal&)=delete;

			struct Impl;
			Impl* m_impl;
		};
	}

#endif
