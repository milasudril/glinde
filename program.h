#ifdef __WAND__
target[name[program.h] type[include]]
dependency[program.o]
#endif

#ifndef ANGLE_PROGRAM_H
#define ANGLE_PROGRAM_H

#include "shader.h"

namespace Angle
	{
	class Program
		{
		public:
			Program():handle(glCreateProgram())
				{}

			Program& shaderAttach(Shader& shader)
				{
				glAttachShader(handle,shader.handle);
				return *this;
				}

			void link()
				{
				glLinkProgram(handle);
				GLint res;
				glGetProgramiv(handle,GL_LINK_STATUS,&res);
				if(res!=GL_TRUE)
					{linkException();}
				}

			GLuint uniformLocationGet(const char* name) const
				{
				return glGetUniformLocation(handle,name);
				}

			void use() const
				{
				glUseProgram(handle);
				}

			~Program()
				{glDeleteProgram(handle);}

		private:
			Program& operator=(const Program&)=delete;
			Program(const Program&)=delete;
			GLuint handle;
			void linkException();
		};
	}

#endif