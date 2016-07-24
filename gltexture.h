//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"GLEW",
//@	            "rel":"external"
//@	        },
//@	        {
//@	            "ref":"GL",
//@	            "rel":"external"
//@	        },
//@	        {
//@	            "ref":"gltexture.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"gltexture.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_GLTEXTURE_H
#define GLINDE_GLTEXTURE_H

#include <GL/glew.h>
#include <utility>

#include <cstdio>

namespace Glinde
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
				}

			void dataSet(const Image& source) noexcept;

			void dataSet(uint32_t width,uint32_t height) noexcept
				{
				bind();
				glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F
					,width,height,0,GL_RGB, GL_UNSIGNED_BYTE,nullptr);
				}

			void filterDisable() noexcept
				{
				bind();
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				}

			void filterEnable() noexcept
				{
				bind();
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
				glGenerateMipmap(GL_TEXTURE_2D);
				}

			void bind() const noexcept
				{
				glBindTexture(GL_TEXTURE_2D,m_id);
				}

			~GlTexture() noexcept
				{
				if(m_id!=0)
					{
					glBindTexture(GL_TEXTURE_2D,0);
					glDeleteTextures(1,&m_id);
					}
				}

		private:
			GLuint m_id;
			friend class GlFramebuffer;
		};
	}

#endif
