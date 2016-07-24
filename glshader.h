//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"GLEW",
//@	            "rel":"external"
//@	        },
//@	        {
//@	            "ref":"glshader.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"glshader.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_GLSHADER_H
#define GLINDE_GLSHADER_H

#include <GL/glew.h>

namespace Glinde
	{
	class GlShader
		{
		public:
			explicit GlShader(GLenum type)
				{
				id=glCreateShader(type);
				}

			GlShader& sourceSet(const char* source) noexcept
				{
				glShaderSource(id,1,&source,NULL);
				return *this;
				}

			void compile();

			~GlShader()
				{glDeleteShader(id);}

			GLuint idGet() const noexcept
				{return id;}

		private:
			GLuint id;
		};
	}

#endif
