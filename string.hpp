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
//@	 "dependencies_extra":[{"ref":"string.o","rel":"implementation"}]
//@	,"targets":[{"name":"string.hpp","type":"include"}]
//@	}
#ifndef GLINDE_STRING_HPP
#define GLINDE_STRING_HPP

#include "storage/arraydynamic.hpp"

namespace Glinde
	{
	class String:private ArrayDynamic<char>
		{
		public:
			using ArrayDynamic<char>::begin;
			using ArrayDynamic<char>::end;

			const char* end() const noexcept
				{return ArrayDynamic<char>::end()-1;}

			char* end() noexcept
				{return ArrayDynamic<char>::end()-1;}

			unsigned int length() const noexcept
				{return ArrayDynamic<char>::length()-1;}


			String()
				{ArrayDynamic<char>::append('\0');}

			explicit String(const char* c_str)
				{
				ArrayDynamic<char>::append('\0');
				append(c_str);
				}

			const char* beginAfter(char ch) const noexcept;

			char* beginAfter(char ch) noexcept;


			String& append(const char* c_str)
				{
				assert(c_str!=begin());
				return appendImpl(c_str);
				}

			String& append(const String& str)
				{
				ArrayDynamic<char>::truncate().append(str);
				return *this;
				}

			String& append(char ch)
				{
				ArrayDynamic<char>::truncate().append(ch).append('\0');
				return *this;
				}

			String& truncate() noexcept
				{
				ArrayDynamic<char>::truncate();
				(*end())='\0';
				return *this;
				}

			String& truncate(char ch) noexcept;

			String& clear() noexcept
				{
				ArrayDynamic<char>::clear();
				ArrayDynamic<char>::append('\0');
				return *this;
				}

		private:
			String& appendImpl(const char* c_str);
		};

		bool operator==(const String& a,const char* b) noexcept;
		bool operator==(const String& a,const String& b) noexcept;

		inline bool operator!=(const String& a,const String& b) noexcept
			{return !(a==b);}

		inline bool operator!=(const String& a,const char* b) noexcept
			{return !(a==b);}
	}

#endif
