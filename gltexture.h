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
				r_source=nullptr;
				glGenTextures(1,&id);
				bind();
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				}

			void dataSet(const Image& source) noexcept;

			void bind()
				{
				glBindTexture(GL_TEXTURE_2D,id);
				}

			~GlTexture()
				{
				glBindTexture(GL_TEXTURE_2D,0);
				glDeleteTextures(1,&id);
				}

		private:
			const Image* r_source;
			GLuint id;
		};
	}

#endif
