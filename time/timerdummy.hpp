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
