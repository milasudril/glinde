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
//@	{"targets":[{"name":"timeinfo.hpp","type":"include"}]}

#ifndef GLINDE_TIMEINFO_HPP
#define GLINDE_TIMEINFO_HPP

#include "wallclocktime.hpp"
#include "../storage/vectortype.hpp"
#include <cstddef>

namespace Glinde
	{
	class Timeinfo
		{
		public:
			explicit Timeinfo(double t,double dt) noexcept:
				m_t{t,dt}
				{}

			double simulationTime() const noexcept
				{return m_t[0];}

			double dt() const noexcept
				{return m_t[1];}

			WallclockTime wallclockTime() const noexcept
				{return m_wct;}
		

		private:
			vec2_t<double> m_t;
			WallclockTime m_wct;
		};
	}

#endif
