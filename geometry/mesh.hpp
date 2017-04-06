//@ {"targets":[{"name":"mesh.hpp","type":"include"}]}

#ifndef GLINDE_MESH_HPP
#define GLINDE_MESH_HPP

#include "faceindirect.hpp"
#include "meshsize.hpp"
#include "../storage/vectortype.hpp"
#include "../storage/range.hpp"
#include <geosimd/point.hpp>
#include <geosimd/vector.hpp>
#include <memory>

namespace Glinde
	{
	class ResourceObject;
	class Mesh
		{
		public:
			explicit Mesh(const ResourceObject& obj);

			auto vertices() const noexcept
				{
				return Range<const GeoSIMD::Point<float>>
					{m_verts.get(),m_size.verticesCount()};
				}

			auto normals() const noexcept
				{
				return Range<const GeoSIMD::Vector<float>>
					{m_normals.get(),m_size.verticesCount()};
				}
			
			auto uvs() const noexcept
				{
				return Range<const vec2_t<float>>
					{m_uvs.get(),m_size.verticesCount()};
				}

			auto faces() const noexcept
				{
				return Range<const FaceIndirect<uint32_t>>
					{m_faces.get(),m_size.facesCount()};
				}

			auto verticesCount() const noexcept
				{return m_size.verticesCount();}

			auto facesCount() const noexcept
				{return m_size.facesCount();}

			auto size() const noexcept
				{return m_size;}

		private:
			std::unique_ptr<GeoSIMD::Point<float>[]> m_verts;
			std::unique_ptr<GeoSIMD::Vector<float>[]> m_normals;
			std::unique_ptr<vec2_t<float>> m_uvs;
			std::unique_ptr<FaceIndirect<uint32_t>> m_faces;
			MeshSize<uint32_t> m_size;
		};
	}

#endif // GLINDE_MESH_HPP
