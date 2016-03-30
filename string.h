#ifdef __WAND__
target[name[string.h] type[include]]
dependency[string.o]
#endif

#ifndef GLINDA_STRING_H
#define GLINDA_STRING_H

#include "vectortype.h"
#include <cstdint>
#include <cstddef>
#include <utility>
#include <cassert>

namespace Glinda
	{
	class String
		{
		public:
			String(uint32_t length):m_content{0,0,0,0}
				{
				auto N=length + 1;
				capacitySet(N);
				m_content.data.length=N;
				*(end())='\0';
				}

			String():m_content{0,0,0,0}
				{
				capacitySet(1);
				m_content.data.length=1;
				*(end())='\0';
				}

			explicit String(const char* c_str):m_content{0,0,0,0}
				{
				capacitySet(1);
				m_content.data.length=1;
				append(c_str);
				}

			String& operator=(const String& str)
				{
				assert(&str!=this);
				String str_copy(str);
				*this=std::move(str_copy);
				return *this;
				}

			String(const String& str):m_content{0,0,0,0}
				{
				capacitySet(str.length() + 1);
				m_content.data.length=1;
				append(str);
				}

			String& operator=(String&& str) noexcept
				{
				assert(&str!=this);
				std::swap(str.m_content.x,m_content.x);
				return *this;
				}

			String(String&& str) noexcept
				{
				m_content.x=str.m_content.x;
				str.m_content.x=vec4_t<int32_t>{0,0,0,0};
				}

			~String();

			const char* begin() const noexcept
				{return m_content.data.pointer;}

			char* begin() noexcept
				{return m_content.data.pointer;}

			const char* end() const noexcept
				{return m_content.data.pointer+length();}

			char* end() noexcept
				{return m_content.data.pointer+length();}

			uint32_t length() const noexcept
				{return m_content.data.length - 1;}

			String& append(const char* c_str)
				{
				assert(c_str!=begin());
				return appendImpl(c_str);
				}

			String& append(const String& str)
				{
				assert(str.begin()!=begin());
				return appendImpl(str);
				}


			String& append(char ch)
				{
				auto content=m_content;
				auto l=length() + 1;
				if(l==content.data.capacity)
					{
				//	TODO: Check for overflow
					capacitySet( l + 1 );
					content.x=m_content.x;
					}

				content.data.pointer[l - 1]=ch;
				content.data.pointer[ l ]='\0';
				++m_content.data.length;
				return *this;
				}

			String& truncate() noexcept
				{
				assert(length()!=0);
				--m_content.data.length;
				m_content.data.pointer[ length() ]='\0';
				return *this;
				}

			String& clear() noexcept
				{
				m_content.data.length=1;
				m_content.data.pointer[ length() ]='\0';
				return *this;
				}

		private:
			union
				{
				vec4_t<int32_t> x;
				struct
					{
					char* pointer;
					uint32_t length;
					uint32_t capacity;
					} data;
				} m_content;

			void capacitySet(size_t N);

			String& appendImpl(const char* c_str);
			String& appendImpl(const String& str);
		};
	}

#endif
