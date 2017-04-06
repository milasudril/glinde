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
//@	 "dependencies_extra":[{"ref":"memoryreader.o","rel":"implementation"}]
//@	,"targets":[{"name":"memoryreader.hpp","type":"include"}]
//@	}

#ifndef GLINDE_MEMORYREADER_HPP
#define GLINDE_MEMORYREADER_HPP

#include "datasource.hpp"
#include "../storage/vectortype.hpp"

#include <cstdint>

namespace Glinde
	{
	class MemoryReader:public DataSource
		{
		public:
			explicit MemoryReader(const uint8_t* begin,const uint8_t* end)
				{m_content.fields={begin,static_cast<size_t>(end-begin)};}

			~MemoryReader()=default;

			const char* nameGet() const noexcept
				{return "internal buffer";}

			size_t read(void* buffer,size_t N);

		private:
			union
				{
				vec2_t<intptr_t> x;
				struct
					{
					const uint8_t* data;
					size_t N;
					} fields;
				} m_content;
		};
	}

#endif
