#ifdef __WAND__
target[name[glelementarraybuffer.h] type[include]]
dependency[GLEW;external]
dependency[GL;external]
#endif

#ifndef GLINDE_GLELEMENTARRAYBUFFER_H
#define GLINDE_GLELEMENTARRAYBUFFER_H

#include "range.h"
#include "faceindirect.h"
#include <GL/glew.h>

namespace Glinde
	{
	class GlElementArrayBuffer
		{
		public:
			GlElementArrayBuffer()
				{glGenBuffers(1,&id);}

			void dataSet(const uint32_t* data,unsigned int N,GLenum usage) noexcept
				{
				m_size=N;
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,id);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER,N*sizeof(uint32_t),data,usage);
				}

			void dataSet(const Range<const FaceIndirect> data,GLenum usage) noexcept
				{
				dataSet(reinterpret_cast<const uint32_t*>( data.begin() )
					,3*static_cast<unsigned int>(data.length()),usage);
				}

			void draw(GLuint index) noexcept
				{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,id);
				glDrawElements(GL_TRIANGLES, m_size,GL_UNSIGNED_INT,(void*)0);
				}

			void drawPoints(GLuint index) noexcept
				{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,id);
				glDrawElements(GL_POINTS, m_size,GL_UNSIGNED_INT,(void*)0);
				}

			~GlElementArrayBuffer()
				{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
				glDeleteBuffers(1,&id);
				}

		private:
			GLuint id;
			unsigned int m_size;
		};
	}

#endif
