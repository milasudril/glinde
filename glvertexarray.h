#ifdef __WAND__
target[name[glvertexarray.h] type[include]]
dependency[GLEW;external]
#endif

#ifndef GLINDE_GLVERTEXARRAY_H
#define GLINDE_GLVERTEXARRAY_H

#include <GL/glew.h>

namespace Glinde
	{
	class GlVertexArray
		{
		public:
			GlVertexArray()
				{glGenVertexArrays(1,&id);}

			void bind() noexcept
				{glBindVertexArray(id);}

			~GlVertexArray()
				{
				glBindVertexArray(0);
				glDeleteVertexArrays(1,&id);
				}

		private:
			GLuint id;
		};
	}
#endif
