#ifdef __WAND__
target[name[arraysimple.h] type[include]]
#endif

#ifndef ARRAYSIMPLE_H
#define ARRAYSIMPLE_H

#include "vectortype.h"
#include "debug.h"
#include "arrayinit.h"
#include "memoryalloc.h"
#include <utility>

namespace Glinda
	{
	template<class T>
	class ArraySimple
		{
		public:
			ArraySimple():m_content{0,0,0,0}
				{}
				
			explicit ArraySimple(size_t n_elems);
			~ArraySimple();

			template<class Initializer>
			explicit ArraySimple(size_t n_elems,Initializer&& initializer);

			ArraySimple(const ArraySimple& a);

			ArraySimple& operator=(const ArraySimple& a);

			ArraySimple(ArraySimple&& a) noexcept
				{
				m_content.x=a.m_content.x;
				a.m_content.x={0,0,0,0};
				}

			ArraySimple& operator=(ArraySimple&& a) noexcept
				{
				std::swap(m_content.x,m_content.x);
				return *this;
				}

			T* begin() noexcept
				{return m_content.fields.data;}

			const T* begin() const noexcept
				{return m_content.fields.data;}

			T* end() noexcept
				{return m_content.fields.data+m_content.fields.N;}

			const T* end() const noexcept
				{return m_content.fields.data+m_content.fields.N;}

			size_t length() const noexcept
				{return m_content.fields.N;}

			const T& operator[](size_t k) const noexcept
				{return m_content.fields.data[k];}

			T& operator[](size_t k) noexcept
				{return m_content.fields.data[k];}

		private:
#if (__amd64 || __x86_64 || __x86_64__ || __amd64__)
			typedef vec4_t<int> TwoPointers;
#else
			typedef vec2_t<int> TwoPointers;
#endif
			union
				{
				TwoPointers x;
				struct
					{
					T* data;
					size_t N;
					} fields;
				} m_content;
		};


	template<class T>
	ArraySimple<T>::ArraySimple(size_t n_elems)
		{
		auto data=reinterpret_cast<T*>(memoryAllocate(n_elems*sizeof(T)));
		try
			{ArrayInit::create(data,data+n_elems);}
		catch(...)
			{
			memoryFree(m_content.fields.data);
			throw;
			}
		m_content.fields.N=n_elems;
		m_content.fields.data=data;
		}

	template<class T>
	template<class Initializer>
	ArraySimple<T>::ArraySimple(size_t n_elems,Initializer&& init)
		{
		auto data=reinterpret_cast<T*>(memoryAllocate(n_elems*sizeof(T)));
		try
			{ArrayInit::create(data,data+n_elems,init);}
		catch(...)
			{
			memoryFree(m_content.fields.data);
			throw;
			}
		m_content.fields.N=n_elems;
		m_content.fields.data=data;
		}


	template<class T>
	ArraySimple<T>::~ArraySimple()
		{
		ArrayInit::destroy(begin(),end());
		memoryFree(m_content.fields.data);
		}

	template<class T>
	ArraySimple<T>& ArraySimple<T>::operator=(const ArraySimple<T>& a)
		{
		assert( &a != this);
		ArraySimple<T> temp(a);
		*this=std::move(temp);
		return *this;
		}

	template<class T>
	ArraySimple<T>::ArraySimple(const ArraySimple& a)
		{
		auto data=reinterpret_cast<T>( memoryAllocate(a.length()*sizeof(T)) );
		try
			{ArrayInit::copy(data,data+a.length(),a.begin());}
		catch(...)
			{memoryFree(data);}
		m_content.fields.N=a.length();
		m_content.fields.data=data;
		}
	}

#endif
