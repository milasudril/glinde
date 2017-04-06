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
//@	 "dependencies_extra":[{"ref":"stringformat.o","rel":"implementation"}]
//@	,"targets":[{"name":"stringformat.hpp","type":"include"}]
//@	}
#ifndef GLINDE_STRINGFORMAT_H
#define GLINDE_STRINGFORMAT_H

#include "variant.hpp"
#include "storage/range.hpp"
#include <initializer_list>
#include <cstddef>

namespace Glinde
	{

	void format(const Range<char>& buffer,const char* format_string
		,const Range<const Variant>& args);

	inline void format(const Range<char>& buffer,const char* format_string
		,const std::initializer_list<Variant>& args)
		{format(buffer,format_string,Range<const Variant>(args.begin(),args.size()));}

	}

#endif
