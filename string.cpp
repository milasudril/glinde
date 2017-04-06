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
//@	{"targets":[{"name":"string.o","type":"object"}]}

#include "string.hpp"
#include "debug.hpp"
#include "errormessage.hpp"
#include <cstring>
#include <cstdlib>
#include <limits>

using namespace Glinde;

String& String::appendImpl(const char* c_str)
	{
	ArrayDynamic<char>::truncate();
	while(*c_str != '\0')
		{
		ArrayDynamic<char>::append(*c_str);
		++c_str;
		}
	ArrayDynamic<char>::append('\0');
	return *this;
	}

bool Glinde::operator==(const String& a,const char* b) noexcept
	{
	if(a.length()==0 && *b!='\0')
		{return 0;}
	return strcmp(a.begin(),b)==0;
	}

bool Glinde::operator==(const String& a,const String& b) noexcept
	{
	if(a.length()!=b.length())
		{return 0;}
	return a==b.begin();
	}

String& String::truncate(char ch) noexcept
	{
	if(length()==0)
		{return *this;}
	auto ptr_begin=begin();
	auto ptr_end=end();
	while(ptr_end!=ptr_begin)
		{
		--ptr_end;
		if(*ptr_end==ch)
			{break;}
		}
	auto N=static_cast<uint32_t>( end() - ptr_end );
	ArrayDynamic<char>::truncate(N);
	(*end())='\0';
	return *this;
	}

const char* String::beginAfter(char ch) const noexcept
	{
	auto ptr_begin=begin();
	auto ptr_end=end();
	while(ptr_begin!=ptr_end)
		{
		if(*ptr_begin==ch)
			{
			++ptr_begin;
			return (ptr_begin!=ptr_end)? ptr_begin: begin();
			}
		++ptr_begin;
		}
	return ptr_begin;
	}

char* String::beginAfter(char ch) noexcept
	{
	auto ptr_begin=begin();
	auto ptr_end=end();
	while(ptr_begin!=ptr_end)
		{
		if(*ptr_begin==ch)
			{
			++ptr_begin;
			return (ptr_begin!=ptr_end)? ptr_begin: begin();
			}
		++ptr_begin;
		}
	return begin();
	}
