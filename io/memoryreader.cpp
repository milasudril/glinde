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
//@	{"targets":[{"name":"memoryreader.o","type":"object"}]}

#include "memoryreader.hpp"
#include <algorithm>
#include <cstring>

using namespace Glinde;

size_t MemoryReader::read(void* buffer,size_t N)
	{
	auto n_read=std::min(N,m_content.fields.N);

	memcpy(buffer,m_content.fields.data,n_read);

	m_content.fields.data+=n_read;
	m_content.fields.N-=n_read;

	return n_read;
	}
