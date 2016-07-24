//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"GLEW",
//@	            "rel":"external"
//@	        },
//@	        {
//@	            "ref":"glprogram.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"glprogram.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_GLPROGRAM_H
#define GLINDE_GLPROGRAM_H

#include "glshader.h"

#include <GL/glew.h>

namespace Glinde
	{
	class GlProgram
		{
		public:
			GlProgram()
				{
				id=glCreateProgram();
				}

			GlProgram& shaderAttach(const GlShader& shader) noexcept
				{
				glAttachShader(id,shader.idGet());
				return *this;
				}

			GlProgram& shaderDetatch(const GlShader& shader) noexcept
				{
				glDetachShader(id,shader.idGet());
				return *this;
				}

			void link();

			void use() noexcept
				{glUseProgram(id);}

			GLuint uniformGet(const char* name) const noexcept
				{return glGetUniformLocation(id,name);}

			~GlProgram()
				{
				glUseProgram(0);
				glDeleteProgram(id);
				}


			GLuint idGet() const noexcept
				{return id;}

		private:
			GLuint id;
		};
	}

#endif
