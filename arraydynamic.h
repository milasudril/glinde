#ifdef __WAND__
target[name[arraydynamic.h] type[include]]
#endif

#ifndef GLINDA_ARRAYDYNAMIC_H
#define GLINDA_ARRAYDYNAMIC_H

#include "memoryalloc.h"
#include "vectortype.h"
#include "arrayinit.h"
#include "debug.h"
#include "errormessage.h"

#include <limits>
#include <cstdint>
#include <cstddef>
#include <utility>
#include <cassert>

namespace Glinda
	{
	template<class T>
	class ArrayDynamic
		{
		public:
			ArrayDynamic():m_content{0,0,0,0}
				{}

			ArrayDynamic(ArrayDynamic&& obj) noexcept
				{
				m_content.x=obj.m_content.x;
				obj.m_content.x=vec4_t<int32_t>{0,0,0,0};
				}

			ArrayDynamic& operator=(ArrayDynamic&& obj) noexcept
				{
				assert(this!=&obj);
				std::swap(m_content.x,obj.m_content.x);
				return *this;
				}

			ArrayDynamic& operator=(const ArrayDynamic& obj)
				{
				assert(this!=&obj);
				ArrayDynamic<T> temp(obj);
				*this=std::move(temp);
				return *this;
				}

			ArrayDynamic(const ArrayDynamic& obj):m_content{0,0,0,0}
				{
				append(obj);
				}

			~ArrayDynamic()
				{
				clear();
				memoryFree(m_content.data.pointer);
				}



			uint32_t length() const noexcept
				{return m_content.data.length;}

			T* begin() noexcept
				{return m_content.data.pointer;}

			const T* begin() const noexcept
				{return m_content.data.pointer;}

			T* end() noexcept
				{return m_content.data.pointer+length();}

			const T* end() const noexcept
				{return m_content.data.pointer+length();}

			T& operator[](uint32_t position) noexcept
				{
				assert(position<length());
				return m_content.data.pointer[position];
				}

			const T& operator[](uint32_t position) const noexcept
				{
				assert(position<length());
				return m_content.data.pointer[position];
				}



			ArrayDynamic& append(const ArrayDynamic& array)
				{
				if(array.length()==0)
					{return *this;}
				return append(array.begin(),array.length());
				}

			ArrayDynamic& append(const T* begin_in,uint32_t length_in)
				{
				assert(begin_in!=begin());
				auto length_new=static_cast<size_t>( length() )+length_in;
				if(capacity() < length_new + 1)
					{resize(length_new + 1);}
				ArrayInit::copy(end(),end()+length_in,begin_in);
				m_content.data.length=static_cast<uint32_t>(length_new);
				return *this;
				}

			ArrayDynamic& append(const T& obj)
				{
				auto length_new=static_cast<size_t>( length() ) + 1;
				if(capacity() < length_new + 1)
					{resize(length_new + 1);}
				*end() = obj;
				m_content.data.length=static_cast<uint32_t>(length_new);
				return *this;
				}

			ArrayDynamic& append(T&& obj)
				{
				auto length_new=length() + 1;
				if(capacity() < length_new + 1)
					{resize(length_new + 1);}
				*end() = std::move(obj);
				m_content.data.length=length_new;
				return *this;
				}

			void truncate()
				{
				assert(length()!=0);
				ArrayInit::destroy(end()-1,end());
				--m_content.data.length;
				}

			void truncate(uint32_t N)
				{
				assert(length() >= N);
				ArrayInit::destroy(end()-N,end());
				m_content.data.length-=N;
				}

			void clear()
				{truncate(length());}

		private:
			union
				{
				vec4_t<int32_t> x;
				struct
					{
					T* pointer;
					uint32_t length;
					uint32_t capacity;
					} data;
				} m_content;


#if ( _WIN32 && (__amd64 || __x86_64 || __x86_64__ || __amd64__) )
			constexpr size_t nextpow2(size_t N)
				{
				return 1<<(8*sizeof(N) - 1 - __builtin_clzll(N) + 1);
				}
#else
			constexpr size_t nextpow2(size_t N)
				{
				return 1<<(8*sizeof(N) - 1 - __builtin_clzl(N) + 1);
				}
#endif

			void resize(size_t N);
			uint32_t capacity() const noexcept
				{return m_content.data.capacity;}
		};


	template<class T>
	void ArrayDynamic<T>::resize(size_t N)
		{
		auto N_2=nextpow2(N);
		if(N_2 > std::numeric_limits<uint32_t>::max())
			{throw ErrorMessage("Block resize to %zu would result in a too large array.",N_2);}
	//	GLINDA_DEBUG_PRINT("Resizing block of size %u to %zu",m_content.data.capacity,N_2);

		T* block_new=reinterpret_cast<T*>(memoryRealloc(begin(),N_2*sizeof(T)));
		memset(block_new+length(),0,N_2*sizeof(T));
		m_content.data.pointer=block_new;
		m_content.data.capacity=static_cast<uint32_t>(N_2);
		}
	}

#endif
