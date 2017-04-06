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
