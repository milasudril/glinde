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
//@	{"targets":[{"name":"strerror.o","type":"object"}]}
#include "strerror.hpp"

#ifdef _GNU_SOURCE

extern "C"
    {
    extern int __xpg_strerror_r(int errcode,char* buffer,size_t length);
    #define strerror_r __xpg_strerror_r
    }

#else

#include <cstring>

#endif

using namespace Glinde;

int Glinde::strerror(int errcode,char* buffer,size_t length)
	{
	auto ret=strerror_r(errcode,buffer,length);
	buffer[length-1]=0;
	return ret;
	}
