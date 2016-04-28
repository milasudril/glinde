#ifdef __WAND__
target[name[mesh.h] type[include]]
dependency[mesh.o]
#endif

#ifndef GLINDA_MESH_H
#define GLINDA_MESH_H

#include "arraysimple.h"
#include "arrayfixed.h"
#include "boundingbox.h"
#include "stringkey.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <map>

namespace Glinda
	{
	class Image;
	class TextureManager;
	class ResourceObject;

	class Mesh
		{
		public:
			struct Face:public ArrayFixed<unsigned int,3>
				{
				using ArrayFixed<unsigned int,3>::ArrayFixed;
				};

			Mesh(TextureManager& textures,const ResourceObject& source
				,const char* source_name);

			~Mesh();

			Range<const glm::vec3*> verticesGet() const noexcept
				{return m_vertices;}

			ArrayFixed<glm::vec3,3> verticesFromFaceGet(const Face& face) const noexcept
				{
				assert(face[0]<m_vertices.length()
					&& face[1]<m_vertices.length()
					&& face[2]<m_vertices.length());
				auto verts=m_vertices.begin();
				return {verts[face[0]],verts[face[1]],verts[face[2]]};
				}

			Range<const glm::vec3*> normalsGet() const noexcept
				{return m_normals;}

			Range<const glm::vec2*> uvsGet() const noexcept
				{return m_uvs;}

			Range<const Face*> facesGet() const noexcept
				{return m_faces;}

			static constexpr unsigned int textureCountGet() noexcept
				{return decltype(r_textures)::length();}

			Range<const Image* const*> texturesGet() const noexcept
				{return r_textures;}

			const BoundingBox& boundingBoxGet() const noexcept
				{return m_box;}



		private:
			BoundingBox m_box;
			ArraySimple<glm::vec3> m_vertices;
			ArraySimple<glm::vec3> m_normals;
			ArraySimple<glm::vec2> m_uvs;
			ArraySimple<Face> m_faces;
			ArrayFixed<const Image*,4> r_textures;

			void boundingBoxUpdate() noexcept;
		};
	}

#endif
