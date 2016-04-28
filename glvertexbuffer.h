#ifdef __WAND__
target[name[glvertexbuffer.h] type[include]]
dependency[GLEW;external]
dependency[GL;external]
#endif

#ifndef GLINDA_GLVERTEXBUFFER_H
#define GLINDA_GLVERTEXBUFFER_H

#include "debug.h"
#include "range.h"
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <cassert>

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

			void dataSet(const Range<const glm::vec3*>& data,GLenum usage) noexcept
				{
				dataSet(reinterpret_cast<const float*>(data.begin())
					,3*static_cast<unsigned int>(data.length()),usage);
				}

			void dataSet(const Range<const glm::vec2*>& data,GLenum usage) noexcept
				{
				dataSet(reinterpret_cast<const float*>(data.begin())
					,2*static_cast<unsigned int>( data.length() ),usage);
				}

			void draw(GLuint index,unsigned int elem_size) noexcept
				{
				assert(elem_size==3);
				attributesBind(index,elem_size);
				glDrawArrays(GL_TRIANGLES, 0, m_size);
				}

			void attributesBind(GLuint index,unsigned int elem_size) noexcept
				{
				assert(m_size%elem_size==0);
				glBindBuffer(GL_ARRAY_BUFFER,id);
				glVertexAttribPointer(index,elem_size,
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
