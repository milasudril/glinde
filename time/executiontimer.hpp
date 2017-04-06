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
				return ret + static_cast<double>(now.tv_nsec - m_start.tv_nsec)/1e9;
				}

			void reset() noexcept
				{clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&m_start);}

		private:
			timespec m_start;
		};
	}

#endif
#endif
