#ifdef __WAND__
target[name[renderer.h] type[include]]
dependency[renderer.o]
#endif

#ifndef GLINDA_RENDERER_H
#define GLINDA_RENDERER_H

#include "glprogram.h"
#include "glvertexarray.h"
#include "glvertexbuffer.h"
#include "glelementarraybuffer.h"
#include "gltexture.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Glinda
	{
	class WorldObject;
	class World;
	class Mesh;

	template<class T> class Range;

	class Renderer
		{
		public:
			Renderer();
			~Renderer();

			void sceneRender(World& world,const WorldObject& viewer) noexcept;
			void viewportSizeSet(int width,int height) noexcept;

		private:
			GlProgram program;

			GlVertexArray array;
			GlVertexBuffer vertices;
			GlVertexBuffer uvs;
			GlVertexBuffer normals;
			GlElementArrayBuffer vertex_indices;
			GlTexture texture;

			glm::mat4 P;
			GLuint MVP_id;
			GLuint V_id;
			GLuint M_id;
			GLuint lightpos_id;
			GLuint diffuse_id;

			void render(const Range< const Mesh* >& meshes) noexcept;
		};
	}

#endif
