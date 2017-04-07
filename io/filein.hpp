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
//@	 "targets":[{"name":"filein.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"filein.o","rel":"implementation"}]
//@	}
#ifndef GLINDE_FILEIN_HPP
#define GLINDE_FILEIN_HPP

#include "datasource.hpp"
#include "../string.hpp"

namespace Glinde
	{
	class FileIn:public DataSource
		{
		public:
			explicit FileIn(const char* filename);
			~FileIn();

			size_t read(void* buffer,size_t count);

			const char* nameGet() const noexcept
				{return m_name.begin();}

		private:
			String m_name;
			intptr_t m_handle;
		};
	}

#endif
