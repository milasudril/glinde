#ifdef __WAND__
target[name[vertex_buffer.h] type[include]]
dependency[vertex_buffer.o]
#endif

#ifndef ANGLE_VERTEXBUFFER_H
#define ANGLE_VERTEXBUFFER_H

#include <GL/glew.h>

namespace Angle
	{
	class VertexBuffer
		{
		public:
			VertexBuffer()
				{glGenBuffers(1,&handle);}

			void bind()
				{glBindBuffer(GL_ARRAY_BUFFER,handle);}

			~VertexBuffer()
				{glDeleteBuffers(1,&handle);}

		private:
			GLuint handle;
		};
	}

#endif
