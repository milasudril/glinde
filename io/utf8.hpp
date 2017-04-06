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
//@	 "targets":[{"name":"utf8.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"utf8.o","rel":"implementation"}]
//@	}

#include "../string.hpp"
#include "../storage/arraydynamic.hpp"
#include <cstdint>

namespace Glinde
	{
	static constexpr uint32_t UTF8_ACCEPT=0;
	static constexpr uint32_t UTF8_REJECT=1;

	uint32_t utf8_decode(uint32_t& state, uint32_t& codep, int8_t byte) noexcept;
	ArrayDynamic<uint32_t> utf8_decode(const char* str);
	String utf8_encode(Range<const uint32_t> src);
	}
