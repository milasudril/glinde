//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"glframebuffer.o",
//@	            "rel":"implementation"
//@	        },
//@	        {
//@	            "ref":"GLEW",
//@	            "rel":"external"
//@	        },
//@	        {
//@	            "ref":"GL",
//@	            "rel":"external"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"glframebuffer.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_GLFRAMEBUFFER_H
#define GLINDE_GLFRAMEBUFFER_H

#include "gltexture.h"

namespace Glinde
	{
	class GlFramebuffer
		{
		public:
			GlFramebuffer()
				{
				glGenFramebuffers(1,&m_handle);
				glGenRenderbuffers(1,&m_depth);
				}

			void targetSet(GlTexture& texture,unsigned int width,unsigned int height);

			void bind()
				{
				glBindFramebuffer(GL_FRAMEBUFFER,m_handle);
				glViewport(0,0,m_width,m_height);
				}

			void unbind()
				{
				glBindFramebuffer(GL_FRAMEBUFFER,0);
				}

			~GlFramebuffer()
				{
				unbind();
				glDeleteFramebuffers(1,&m_handle);
				glDeleteRenderbuffers(1,&m_depth);
				}
		private:
			GLuint m_handle;
			GLuint m_depth;
			unsigned int m_width;
			unsigned int m_height;
		};
	}

#endif
