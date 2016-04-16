#ifdef __WAND__
target[name[timer.h] type[include]]
dependency[timer.o]
#endif

/**\file timer.h \brief Defines a periodic timer.
 */

#ifndef GLINDA_TIMER_H
#define GLINDA_TIMER_H

#include <cstdint>

namespace Glinda
	{
	/**\brief A periodic timer that ticks at predefined frequency.
	 *
	 * Timer objects releases an event with a given frequency.
	 *
	 */
	class Timer
		{
		public:
			/**\brief Initializes the timer that makes it tick with the given
			 * frequency.
			 *
			 * This constructor initializes the timer that makes it tick with
			 * the given frequency.
			 *
			 * \param frequency The event frequency given in Hz.
			 *
			*/
			Timer(double frequency);
			~Timer();

			/**\brief Blocks the calling thread until the timer has made a tick.
			 *
			 * This function blocks the calling thread until the timer has made
			 * a tick.
			 *
			*/
			void wait() const noexcept;

			/**\brief Gets the time interval in seconds between two frames.
			 *
			 * This function returns the number of seconds between two frames.
			*/
			double delayGet() const noexcept;


		private:
			struct Impl;
			Impl* m_impl;
		};
	}

#endif
