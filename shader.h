#ifdef __WAND__
target[name[shader.h] type[include]]
dependency[shader.o]
#endif

#ifndef ANGLE_SHADER_H
#define ANGLE_SHADER_H

#include <GL/glew.h>

namespace Angle
	{
	class Shader
		{
		public:
			Shader(GLuint type):handle(glCreateShader(type))
				{}

			Shader& sourceSet(const uint8_t* source,size_t length)
				{
				GLint l_trunc=length;
				glShaderSource(handle,1,(const char**)&source,&l_trunc);
				return *this;
				}

			void compile()
				{
				glCompileShader(handle);
				GLint res;
				glGetShaderiv(handle,GL_COMPILE_STATUS,&res);
				if(res!=GL_TRUE)
					{compileException();}
				}

			~Shader()
				{glDeleteShader(handle);}

		private:
			Shader& operator=(const Shader&)=delete;
			Shader(const Shader&)=delete;
			GLuint handle;

			friend class Program;
			void compileException();
		};
	}

#endif