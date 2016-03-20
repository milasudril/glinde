#ifdef __WAND__
target[name[glvertexarray.h] type[include]]
dependency[GLEW;external]
#endif

#ifndef GLINDA_GLVERTEXARRAY_H
#define GLINDA_GLVERTEXARRAY_H

#include <GL/glew.h>

namespace Glinda
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
