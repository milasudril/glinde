#ifdef __WAND__
target[name[vertex_array.h] type[include]]
dependency[vertex_array.o]
#endif

#ifndef ANGLE_VERTEXARRAY_H
#define ANGLE_VERTEXARRAY_H

#include <GL/glew.h>

namespace Angle
	{
	class VertexArray
		{
		public:
			VertexArray()
				{glGenVertexArrays(1,&handle);}

			void bind()
				{glBindVertexArray(handle);}

			~VertexArray()
				{glDeleteVertexArrays(1,&handle);}

		private:
			GLuint handle;
		};
	}

#endif
