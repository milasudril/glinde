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
			virtual void wait() noexcept=0;
			virtual double delay() const noexcept=0;
		};
	}

#endif
