//@	{"targets":[{"name":"vertexarray.hpp","type":"include"}]}

#ifndef ANGLE_VERTEXARRAY_HPP
#define ANGLE_VERTEXARRAY_HPP

#include "arraysize.hpp"
#include "valuetype.hpp"
#include <utility>
#include <cassert>

namespace Angle
	{
	template<GLsizei index,class T>
	GLuint get(const T&);

	template<class BatchLayout>
	class VertexArray
		{
		public:
			struct Attribute
				{
				GLint elem_count;
				ValueType type;
				bool normalized;
				GLuint offset;
				};

			VertexArray(const VertexArray&)=delete;

			VertexArray(VertexArray&& obj) noexcept:m_handle(obj.m_handle)
				{obj.m_handle=0;}

			VertexArray& operator=(const VertexArray&)=delete;

			VertexArray operator=(VertexArray&& obj)
				{
				std::swap(obj.m_handle,m_handle);
				return *this;
				}


			VertexArray() noexcept
				{
				assert(glGenVertexArrays!=nullptr);
				glCreateVertexArrays(1,&m_handle);
				for(GLuint k=0;k<size(BatchLayout::attributes);++k)
					{
					const auto& attribute=BatchLayout::attributes[k];
					glVertexArrayAttribFormat(m_handle,k
						,attribute.elem_count
						,native_type(attribute.type)
						,attribute.normalized
						,attribute.offset);
					}
				}
				
			~VertexArray()
				{
				glBindVertexArray(0);
				glDeleteVertexArrays(1,&m_handle);
				}

			void bind() noexcept
				{glBindVertexArray(m_handle);}

			template<GLuint attrib>
			void enableVertexAttribArray() noexcept
				{
				static_assert(attrib>=0 && attrib<size(BatchLayout::attributes),"Attribute index out of bounds");
				glEnableVertexArrayAttrib(m_handle,attrib);
				}

			template<GLuint attrib>
			void disableVertexAttribArray() noexcept
				{
				static_assert(attrib>=0 && attrib<size(BatchLayout::attributes),"Attribute index out of bounds");
				glDisableVertexArrayAttrib(m_handle,attrib);
				}

			template<GLsizei attrib>
			class AttribContext
				{
				public:
					explicit AttribContext(VertexArray& vao) noexcept:
						r_vao(vao)
						{r_vao.enableVertexAttribArray<attrib>();}
					~AttribContext() noexcept
						{r_vao.disableVertexAttribArray<attrib>();}
				private:
					VertexArray& r_vao;
				};

		private:
			GLuint m_handle;
		};
	}

#endif