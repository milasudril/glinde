/*
Glinde
Copyright (C) 2017  Torbjörn Rathsman

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/
//@	{
//@	 "dependencies_extra":[{"ref":"timerreal.o","rel":"implementation"}]
//@	,"targets":[{"name":"timerreal.h","type":"include"}]
//@	}

/**\file timerreal.h \brief Defines a periodic timer.
 */

#ifndef GLINDE_TIMERREAL_HPP
#define GLINDE_TIMERREAL_HPP

#include "timer.hpp"
#include <utility>

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

			TimerReal(TimerReal&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			TimerReal& operator=(TimerReal&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}


		private:
			TimerReal(const TimerReal&)=delete;
			TimerReal& operator=(const TimerReal&)=delete;

			struct Impl;
			Impl* m_impl;
		};
	}

#endif
