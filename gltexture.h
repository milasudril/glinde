#ifdef __WAND__
target[name[gltexture.h] type[include]]
dependency[GLEW;external]
dependency[GL;external]
dependency[gltexture.o]
#endif

#ifndef GLINDA_GLTEXTURE_H
#define GLINDA_GLTEXTURE_H

#include <GL/glew.h>

namespace Glinda
	{
	class Image;

	class GlTexture
		{
		public:
			GlTexture()
				{
				glGenTextures(1,&id);
				}

			void dataSet(const Image& source) noexcept;

			void bind(GLuint index)
				{
				glBindTexture(GL_TEXTURE_2D,id);
				}

			~GlTexture()
				{
				glBindTexture(GL_TEXTURE_2D,0);
				glDeleteTextures(1,&id);
				}

		private:
			GLuint id;
		};
	}

#endif
