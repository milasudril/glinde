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
//@ {"targets":[{"name":"meshsize.hpp","type":"include"}]}

#ifndef GLINDE_MESHSIZE_HPP
#define GLINDE_MESHSIZE_HPP

namespace Glinde
	{
	template<class T>
	class MeshSize
		{
		public:
			explicit constexpr MeshSize(T vert_count,T face_count) noexcept:
				m_verts(vert_count),m_faces(face_count)
				{}

			constexpr T facesCount() const noexcept
				{return m_faces;}

			constexpr T verticesCount() const noexcept
				{return m_verts;}
		private:
			T m_verts;
			T m_faces;
		};
	}

#endif // GLINDE_MESHSIZE_HPP
