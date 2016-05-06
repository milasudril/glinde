#ifdef __WAND__
target[name[shader.h] type[include]]
dependency[GLEW;external]
dependency[glshader.o]
#endif

#ifndef GLINIE_GLSHADER_H
#define GLINIE_GLSHADER_H

#include <GL/glew.h>

namespace Glinie
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
