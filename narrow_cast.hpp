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
//@	{"targets":[{"name":"narrow_cast.hpp","type":"include"}]}

#ifndef GLINDE_NARROWCAST_HPP
#define GLINDE_NARROWCAST_HPP

#include "errormessage.hpp"
#include "variant.hpp"
#include <limits>
#include <type_traits>

namespace Glinde
	{
	template<class T,class U>
	typename std::enable_if<std::is_unsigned<T>::value,T>::type
	narrow_cast(const U& x)
		{
		auto ymax=std::numeric_limits<T>::max();
		if(x<0
			|| static_cast<typename std::make_unsigned<T>::type>(x) > ymax)
			{throw ErrorMessage("A quantity is outside its valid range.",{});}

		return static_cast<T>(x);
		}

	template<class T,class U>
	typename std::enable_if<std::is_signed<T>::value,T>::type
	narrow_cast(const U& x)
		{
		if(x>std::numeric_limits<T>::max() || x<std::numeric_limits<T>::min())
			{throw ErrorMessage("A quantity is outside its valid range.",{});}

		return static_cast<T>(x);
		}
	}

#endif
