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
//@	{"targets":[{"name":"debug.o","type":"object"}]}

#ifndef NDEBUG

#include "debug.hpp"
#include "stringformat.hpp"
#include "variant.hpp"

#include <cstdio>

void Glinde::logWriteDebug(const char* file,uint64_t line
	,const char* format_string
	,const std::initializer_list<Variant>& args) noexcept
	{
	char buffer_a[4096];
	format(Range<char>(buffer_a,sizeof(buffer_a)/sizeof(buffer_a[0]))
		,format_string,args);

	char buffer_b[4096];
	format(Range<char>(buffer_b,sizeof(buffer_b)/sizeof(buffer_b[0]))
		,"(DEBUG) #0;:#1;: #2;\n",{file,line,buffer_a});
	fputs(buffer_b,stderr);
	}

#endif
