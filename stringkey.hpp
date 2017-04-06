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
//@	{"targets":[{"name":"stringkey.h","type":"include"}]}
#ifndef GLINDE_STRINGKEY_HPP
#define GLINDE_STRINGKEY_HPP

#include <cstdint>

namespace Glinde
	{
	class Stringkey
		{
		public:
			typedef uint64_t HashValue;

			explicit constexpr Stringkey(const char* str) noexcept:
				m_value(hash(str))
				{}

			constexpr bool operator<(const Stringkey& key) const noexcept
				{return m_value < key.m_value;}

			constexpr bool operator==(const Stringkey& key) const noexcept
				{return key.m_value==m_value;}

			constexpr bool operator!=(const Stringkey& key) const noexcept
				{return !(key==*this);}

			constexpr operator HashValue() const noexcept
				{return m_value;}

		private:
			HashValue m_value;

			static constexpr uint64_t OFFSET_BASIS=0xcbf29ce484222325;
			static constexpr uint64_t FNV_PRIME=0x100000001b3;
			static constexpr uint64_t hash(const char* str
				,HashValue value_init=OFFSET_BASIS)
				{
				return *str=='\0'?
					 value_init
					:hash(str+1, (value_init^(HashValue(*str)))*FNV_PRIME);
				}
		};
	}

#endif
