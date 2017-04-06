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
//@	{"targets":[{"name":"faceindirect.hpp","type":"include"}]}

#ifndef GLINDE_FACEINDIRECT_HPP
#define GLINDE_FACEINDIRECT_HPP

#include "../storage/arrayfixed.hpp"
#include <cstdint>

namespace Glinde
	{
	template<class T=uint16_t>
	struct FaceIndirect:public ArrayFixed<T,3>
		{
		using ArrayFixed<T,3>::ArrayFixed;
		};

	template<class T>
	inline const T* native_type(const FaceIndirect<T>* x) noexcept
		{return reinterpret_cast<const T*>(x);}
	}


#endif
