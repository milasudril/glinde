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
//@	{"targets":[{"name":"meshhosted.hpp","type":"include"}]}

#ifndef GLINDE_MESHHOSTED_HPP
#define GLINDE_MESHHOSTED_HPP

#include "../geometry/mesh.hpp"
#include "../angle/vertexbuffer.hpp"

namespace Glinde
	{
	template<class T>
	class MeshHosted
		{
		public:
			explicit MeshHosted(const Mesh& m):
				 m_vertices(m.verticesCount())
				,m_normals(m.verticesCount())
				,m_uvs(m.verticesCount())
				,m_faces(m.facesCount())
				{
				auto f=m.faces();
				m_faces.bufferData(f.begin(),f.length());

				auto uv=m.uvs();
				m_uvs.bufferData(uv.begin(),uv.length());

				auto n=m.normals();
				m_normals.bufferData(native_type(n.begin()),n.length());

				auto v=m.vertices();
				m_vertices.bufferData(native_type(v.begin()),v.length());
				}

			const auto& vertices() const noexcept
				{return m_vertices;}

			const auto& normals() const noexcept
				{return m_normals;}

			const auto& uvs() const noexcept
				{return m_uvs;}

			const auto& faces() const noexcept
				{return m_faces;}

		private:
			Angle::VertexBuffer<vec4_t<float>> m_vertices;
			Angle::VertexBuffer<vec4_t<float>> m_normals;
			Angle::VertexBuffer<vec2_t<float>> m_uvs;
			Angle::VertexBuffer<T> m_faces;
		};
	}

#endif // GLINDE_MESHHOSTED_HPP
