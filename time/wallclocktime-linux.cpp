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
//@	[
//@		[
//@		"linux"
//@			,{
//@	 		"targets":
//@				[{"name":"wallclocktime.o","type":"object"}]
//@			 }
//@		]
//@	]

#include "wallclocktime.hpp"
#include <time.h>
#include <type_traits>

using namespace Glinde;

WallclockTime::WallclockTime() noexcept:m_value(time(NULL))
	{}


std::enable_if_t<sizeof(time_t)==sizeof(int64_t),Timestamp>
WallclockTime::timestampUTC() const noexcept
	{
	Timestamp ret;
	tm st;
	gmtime_r(&m_value,&st);
	ret.sec(st.tm_sec)
		.min(st.tm_min)
		.hour(st.tm_hour)
		.day(st.tm_mday)
		.month(st.tm_mon+1)
		.year(st.tm_year+1900);
	return ret;	
	}

std::enable_if_t<sizeof(time_t)==sizeof(int64_t),Timestamp>
WallclockTime::timestampLocal() const noexcept
	{
	Timestamp ret;
	tm st;
	localtime_r(&m_value,&st);
	ret.sec(st.tm_sec)
		.min(st.tm_min)
		.hour(st.tm_hour)
		.day(st.tm_mday)
		.month(st.tm_mon+1)
		.year(st.tm_year+1900);
	return ret;	
	}