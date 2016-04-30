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
			struct FaceIndirect:public ArrayFixed<unsigned int,3>
				{
				using ArrayFixed<unsigned int,3>::ArrayFixed;
				};

			struct Face:public ArrayFixed<glm::vec3,3>
				{
				using ArrayFixed<glm::vec3,3>::ArrayFixed;
				};

			class FaceIterator
				{
				public:
					FaceIterator& operator++() noexcept
						{
						++r_position;
						return *this;
						}

					FaceIterator operator++(int) noexcept
						{
						FaceIterator temp(*this);
						++r_position;
						return temp;
						}

					bool operator==(const FaceIterator& i) const noexcept
						{return i.r_position==r_position && r_vertices==i.r_vertices;}

					bool operator!=(const FaceIterator& i) const noexcept
						{return !(*this==i);}

					Face operator*() const noexcept
						{
						auto face=*r_position;
						auto verts=r_vertices;
						return {verts[face[0]],verts[face[1]],verts[face[2]]};
						}

				private:
					FaceIterator(const glm::vec3* vertices
						,const FaceIndirect* position) noexcept:
						r_vertices(vertices),r_position(position){}

					friend class Mesh;

					const glm::vec3* r_vertices;
					const FaceIndirect* r_position;
				};

			Mesh(TextureManager& textures,const ResourceObject& source
				,const char* source_name);

			~Mesh();

			Range<const glm::vec3*> verticesGet() const noexcept
				{return m_vertices;}

			Range<const glm::vec3*> normalsGet() const noexcept
				{return m_normals;}

			Range<const glm::vec2*> uvsGet() const noexcept
				{return m_uvs;}

			Range<const FaceIndirect*> facesIndirectGet() const noexcept
				{return m_faces;}

			FaceIterator facesBegin() const noexcept
				{return FaceIterator(m_vertices.begin(),m_faces.begin());}

			FaceIterator facesEnd() const noexcept
				{return FaceIterator(m_vertices.begin(),m_faces.end());}

			static constexpr unsigned int textureCountGet() noexcept
				{return decltype(r_textures)::length();}

			Range<const Image* const*> texturesGet() const noexcept
				{return r_textures;}

			const BoundingBox& boundingBoxGet() const noexcept
				{return m_box;}

			glm::vec3 vertexGet(size_t k) const noexcept
				{return m_vertices[k];}




		private:
			BoundingBox m_box;
			ArraySimple<glm::vec3> m_vertices;
			ArraySimple<glm::vec3> m_normals;
			ArraySimple<glm::vec2> m_uvs;
			ArraySimple<FaceIndirect> m_faces;
			ArrayFixed<const Image*,4> r_textures;

			void boundingBoxUpdate() noexcept;
			friend class FaceIterator;
		};

	BoundingBox boundingBoxGet(const Range<const Mesh*>& meshes);
	bool intersect(const Mesh::Face& a,const Mesh::Face& b);
	bool intersect(const Mesh::Face& a,const Mesh& mesh);
	bool intersect(const Mesh::Face& a,const Range<const Mesh*>& meshes);

	inline Mesh::Face operator+(const Mesh::Face& face,const glm::vec3& x)
		{
		auto ret=face;
		for(size_t k=0;k<face.length();++k)
			{ret[k]+=x;}
		return ret;
		}

	inline Mesh::Face operator-(const Mesh::Face& face,const glm::vec3& x)
		{
		auto ret=face;
		for(size_t k=0;k<face.length();++k)
			{ret[k]-=x;}
		return ret;
		}
	}

#endif
