#ifdef __WAND__
target[name[gltexture.h] type[include]]
dependency[GLEW;external]
dependency[GL;external]
dependency[gltexture.o]
#endif

#ifndef GLINIE_GLTEXTURE_H
#define GLINIE_GLTEXTURE_H

#include <GL/glew.h>
#include <utility>

namespace Glinie
	{
	class Image;

	class GlTexture
		{
		public:
			GlTexture(const GlTexture&)=delete;
			GlTexture& operator=(const GlTexture&)=delete;

			GlTexture(GlTexture&& obj) noexcept:m_id(obj.m_id)
				{obj.m_id=0;}

			GlTexture& operator=(GlTexture&& obj) noexcept
				{
				std::swap(m_id,obj.m_id);
				return *this;
				}

			GlTexture()
				{
				glGenTextures(1,&m_id);
				bind();
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
				}

			void dataSet(const Image& source) noexcept;

			void bind()
				{
				glBindTexture(GL_TEXTURE_2D,m_id);
				}

			~GlTexture()
				{
				if(m_id!=0)
					{
					glBindTexture(GL_TEXTURE_2D,0);
					glDeleteTextures(1,&m_id);
					}
				}

		private:
			GLuint m_id;
		};
	}

#endif
