//@	{"targets":[{"name":"vertexarray.hpp","type":"include"}]}

#ifndef ANGLE_VERTEXARRAY_HPP
#define ANGLE_VERTEXARRAY_HPP

#include "valuetype.hpp"
#include <utility>
#include <cassert>

namespace Angle
	{
	template<GLsizei N>
	class VertexArrays
		{
		public:
			template<GLsizei k>
			struct Index
				{static constexpr GLsizei value=k;};

			VertexArrays() noexcept
				{
				assert(glGenVertexArrays!=nullptr);
				glGenVertexArrays(N,m_handles);
				for_each([](auto vaos,auto index)
					{assert(vaos.m_handles[decltype(index)::value]!=0);});
				}
				
			~VertexArrays()
				{
				glBindVertexArray(0);
				glDeleteVertexArrays(N,m_handles);
				}

			template<GLsizei index=0>
			void bind() noexcept
				{
				static_assert(index>=0 && index<N,"Index out of bounds");
				glBindVertexArray(m_handles[index]);
				}

			template<GLsizei index=0>
			void enableVertexAttribArray(GLuint attrib) noexcept
				{
				static_assert(index>=0 && index<N,"Index out of bounds");
				glEnableVertexAttribArray(m_handles[index],attrib);
				}

			template<GLsizei index=0>
			void disableVertexAttribArray(GLuint attrib) noexcept
				{
				static_assert(index>=0 && index<N,"Index out of bounds");
				glDisableVertexAttribArray(m_handles[index],attrib);
				}

			template<GLsizei index=0>
			void vertexArrayAttribFormat(GLuint attrib
				,GLint value_count
				,ValueType type,bool normalized,GLuint offset) noexcept
				{
				static_assert(index>=0 && index<N,"Index out of bounds");
				glVertexArrayAttribFormat(m_handles[index],attrib,value_count
					,native_type(type),normalized,offset);
				}

			template<class T>
			void for_each(T&& callback) noexcept
				{
				auto cb_temp=std::move(callback);
				RunLoop<N,T>::doIt(cb_temp,*this);
				}

		private:
			template<GLsizei k,class T>
			struct RunLoop
				{
				static void doIt(T& callback,VertexArrays<N>& self)
					{
					callback(self,Index<k>{});
					RunLoop<k-1,T>::doIt(callback,self);
					}
				};
			template<class T>
			struct RunLoop<0,T>
				{
				static void doIt(const T&,const VertexArrays<N>&){}
				};

			GLuint m_handles[N];
		};

	class VertexArray:public VertexArrays<1>
		{};
	}

#endif