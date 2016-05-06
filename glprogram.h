#ifdef __WAND__
target[name[program.h] type[include]]
dependency[GLEW;external]
dependency[glprogram.o]
#endif

#ifndef GLINIE_GLPROGRAM_H
#define GLINIE_GLPROGRAM_H

#include "glshader.h"

#include <GL/glew.h>

namespace Glinie
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
