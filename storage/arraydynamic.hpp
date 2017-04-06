/*
Glinde
Copyright (C) 2017  Torbjörn Rathsman

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/
//@	{"targets":[{"name":"arraydynamic.hpp","type":"include"}]}

/**\file arraydynamic.hpp \brief Definition of an array that can grow
 */

#ifndef GLINDE_ARRAYDYNAMIC_HPP
#define GLINDE_ARRAYDYNAMIC_HPP

#include "vectortype.hpp"
#include "arrayinit.hpp"
#include "range.hpp"
#include "../variant.hpp"
#include "../debug.hpp"
#include "../errormessage.hpp"
#include "../memory/memoryalloc.hpp"

#include <limits>
#include <cstdint>
#include <cstddef>
#include <utility>
#include <cassert>
#include <new>

namespace Glinde
	{
	/**\brief Class representing arrays that can grow.
	 *
	 * This class describes an array type that can grow. Like ArraySimple, the
	 * memory allocation is done through the functions declared in memoryalloc.h
	 * , which makes it possible to use SSE instructions on its elements.
	 *
	 */
	template<class T>
	class ArrayDynamic
		{
		public:
			/**\brief Default constructor.
			 *
			 * This is the default constructor. When the array is initialized
			 * , it is empty.
			*/
			ArrayDynamic() noexcept:m_content{0,0,0,0}
				{}

			/**\brief Move constructor.
			*/
			ArrayDynamic(ArrayDynamic&& obj) noexcept
				{
				m_content.x=obj.m_content.x;
				obj.m_content.x=vec4_t<int32_t>{0,0,0,0};
				}

			/**\brief Move assignment operator.
			*/
			ArrayDynamic& operator=(ArrayDynamic&& obj) noexcept
				{
				assert(this!=&obj);
				std::swap(m_content.x,obj.m_content.x);
				return *this;
				}

			/**\brief Copy assignment operator.
			 *
			 * This is the copy assignment operator.
			 *
			 * \note Copy assignment requires that T objects can be copy constructed.
			 *
			 */
			ArrayDynamic& operator=(const ArrayDynamic& obj)
				{
				assert(this!=&obj);
				ArrayDynamic<T> temp(obj);
				*this=std::move(temp);
				return *this;
				}

			/**\brief Copy constructor.
			 *
			 * This is the copy constructor.
			 *
			 * \note Copy constuction requires that T objects can be copy constructed.
			 *
			 */
			ArrayDynamic(const ArrayDynamic& obj):m_content{0,0,0,0}
				{
				append(obj);
				}

			~ArrayDynamic()
				{
				clear();
				memoryFree(m_content.data.pointer);
				}


			/**\brief Returns the length of the array.
			*/
			uint32_t length() const noexcept
				{return m_content.data.length;}

			/**\brief Returns a pointer to the first elemenet in the array.
			*/
			T* begin() noexcept
				{return m_content.data.pointer;}

			/**\brief Returns a pointer to the first elemenet in the array.
			*/
			const T* begin() const noexcept
				{return m_content.data.pointer;}

			/**\brief Returns a pointer to the end of the array.
			*/
			T* end() noexcept
				{return m_content.data.pointer+length();}

			/**\brief Returns a pointer to the end of the array.
			*/
			const T* end() const noexcept
				{return m_content.data.pointer+length();}

			/**\brief Returns a reference to the object at <var>position</var>.
			 */
			T& operator[](uint32_t position) noexcept
				{
				assert(position<length());
				return m_content.data.pointer[position];
				}

			/**\brief Returns a reference to the object at <var>position</var>.
			 */
			const T& operator[](uint32_t position) const noexcept
				{
				assert(position<length());
				return m_content.data.pointer[position];
				}

			/**\brief Array range.
			 *
			 * This function returns a Range representing the content of the
			 * array.
			 */
			operator Range<const T>() const noexcept
				{
				return Range<const T>(begin(),length());
				}

			/**\brief Array range.
			 *
			 * This function returns a Range representing the content of the
			 * array.
			 */
			operator Range<T>() noexcept
				{
				return Range<T>(begin(),length());
				}

			/**\brief Appends another array object to the array.
			 *
			 * This function appends another array object to the array.
			 *
			* \note Copy constuction requires that T objects can be copy constructed.
			 *
			 * \warning This function may move the array to a new base address,
			 * and therefore, pointers to elements in the array may become invalid.
			 */
			ArrayDynamic& append(const Range<const T>& array)
				{
				assert(array.length()<=0xffffffffu);
				if(array.length()==0)
					{return *this;}
				return append(array.begin(),static_cast<uint32_t>(array.length()));
				}

			/**\brief Appends another array object to the array.
			 *
			 * This function appends another array object to the array.
			 *
			 * \note Copy constuction requires that T objects can be copy constructed.
			 *
			 * \warning This function may move the array to a new base address,
			 * and therefore, pointers to elements in the array may become invalid.
			 */
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

			/**\brief Appends an object to the array.
			 *
			 * This function appends an object to the array.
			 *
			 * \note Copy constuction requires that T objects can be copy constructed.
			 *
			 * \warning This function may move the array to a new base address,
			 * and therefore, pointers to elements in the array may become invalid.
			 */
			ArrayDynamic& append(const T& obj)
				{
				auto length_new=static_cast<size_t>( length() ) + 1;
				if(capacity() < length_new)
					{resize(length_new);}
				new(end())T(obj);
				m_content.data.length=static_cast<uint32_t>(length_new);
				return *this;
				}

			/**\brief  Appends an object to the array by move.
			 *
			 * This function appends an object to the array by moving it.
			 *
			 * \warning This function may move the array to a new base address,
			 * and therefore, pointers to elements in the array may become invalid.
			 */
			ArrayDynamic& append(T&& obj)
				{
				auto length_new=length() + 1;
				if(capacity() < length_new)
					{resize(length_new);}
				new(end())T(std::move(obj));
				m_content.data.length=length_new;
				return *this;
				}

			/**\brief  Removes the last element from the array.
			 *
			 * This function removes the last element from the array. It has the
			 * same effect as
			 *
			 * \code{.cpp}
			 * truncate(1);
			 * \endcode
			 *
			 */
			ArrayDynamic& truncate()
				{
				assert(length()!=0);
				ArrayInit::destroy(end()-1,end());
				--m_content.data.length;
				return *this;
				}

			/**\brief  Removes the <var>N</var> last element from the array.
			 */
			ArrayDynamic& truncate(uint32_t N)
				{
				assert(length() >= N);
				ArrayInit::destroy(end()-N,end());
				m_content.data.length-=N;
				return *this;
				}

			/**\brief Removes all elements from the array.
			 *
			 * This function removes all elements from the array. It has the
			 * same effect as
			 *
			 * \code{.cpp}
			 * truncate(length());
			 * \endcode
			 *
			 */
			ArrayDynamic& clear()
				{return truncate(length());}

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
			{
			throw ErrorMessage("Block resize to #0; would result in a to large array."
				,{N_2});
			}
	//	GLINDE_DEBUG_PRINT("Resizing block of size %u to %zu",m_content.data.capacity,N_2);

		T* block_new=reinterpret_cast<T*>(memoryRealloc(begin(),N_2*sizeof(T)));
		m_content.data.pointer=block_new;
		m_content.data.capacity=static_cast<uint32_t>(N_2);
		}
	}

#endif
