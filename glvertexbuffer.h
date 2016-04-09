#ifdef __WAND__
target[name[glvertexbuffer.h] type[include]]
dependency[GLEW;external]
dependency[GL;external]
#endif

#ifndef GLINDA_GLVERTEXBUFFER_H
#define GLINDA_GLVERTEXBUFFER_H

#include <GL/glew.h>

namespace Glinda
	{
	class GlVertexBuffer
		{
		public:
			GlVertexBuffer()
				{glGenBuffers(1,&id);}

			void dataSet(const float* data,unsigned int size,GLenum usage) noexcept
				{
				m_size=size;
				glBindBuffer(GL_ARRAY_BUFFER,id);
				glBufferData(GL_ARRAY_BUFFER,size*sizeof(float),data,usage);
				}

			void draw(GLuint index) noexcept
				{
				attributesBind(index);
				glDrawArrays(GL_TRIANGLES, 0, m_size);
				}

			void attributesBind(GLuint index)
				{
				glBindBuffer(GL_ARRAY_BUFFER,id);
				glVertexAttribPointer(index,3,
					GL_FLOAT,// type
					GL_FALSE,// normalized?
					0,// stride
					(void*)0// array buffer offset
					);
				}

			~GlVertexBuffer()
				{
				glBindBuffer(GL_ARRAY_BUFFER,0);
				glDeleteBuffers(1,&id);
				}

		private:
			GLuint id;
			unsigned int m_size;
		};
	}

#endif
