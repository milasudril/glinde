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

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Glinda
	{
	class World;
	class WorldObject;

	class Renderer
		{
		public:
			Renderer();
			~Renderer();

			void sceneRender(World& world,WorldObject& camera) noexcept;
			void viewportSizeSet(int width,int height) noexcept;

		private:
			GlProgram program;

			GlVertexArray array;
			GlVertexBuffer vertices;
			GlElementArrayBuffer vertex_indices;
			glm::mat4 P;
			GLuint MVP_id;
		};
	}

#endif
