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
//@	{"targets":[{"name":"arraysimple.hpp","type":"include"}]}

/** \file arraysimple.hpp \brief Definition of ArraySimple
*/

#ifndef GLINDE_ARRAYSIMPLE_HPP
#define GLINDE_ARRAYSIMPLE_HPP

#include "vectortype.hpp"
#include "arrayinit.hpp"
#include "range.hpp"
#include "../debug.hpp"
#include "../memory/memoryalloc.hpp"

#include <utility>
#include <cstdint>
#include <cstdio>

namespace Glinde
	{
	/**\brief Class representing non-growing dynamically allocated arrays.
	 *
	 * This class describes non-growing dynamically allocated arrays.
	 * Like ArrayDynamic, the memory allocation is done through the functions
	 * declared in memoryalloc.h , which makes it possible to use SSE
	 * instructions on its elements.
	 *
	 */
	template<class T>
	class ArraySimple
		{
		public:
			~ArraySimple();

			/**\brief Default constructor.
			 *
			 * This is the default constructor. An ArraySimple, that has been
			 * initialized by this constructor, is empty.
			*/
			ArraySimple() noexcept:m_content{0,0}
				{}

			/**\brief Size initialize constructor.
			 *
			 * This constructor initializes the array with a capacity of
			 * <var>n_elems</var> elements. The elements allocated will be default
			 * initialized.
			*/
			explicit ArraySimple(size_t n_elems);

			/**\brief Element initialize constructor.
			 *
			 * This constructor initializes the array with a capacity of
			 * <var>n_elems</var> elements. The elements allocated will be initialized
			 * using the function object given by <var>initializer</var>. This function
			 * object must behave as a ArrayInit::ElementInitializer.
			 *
			 * The main usecase for this function is to create an array of
			 * objects that cannot be default initialized. This is the case when
			 * dealing with system resources, that must have a valid handle,
			 * like files. To create an array of files, the following piece of
			 * code can be used:
			 *
			 * \code{.cpp}
			 * // Assume there is a class called File, and another array object
			 * // containing all filenames
			 *
			 * Glinde::ArraySimple<File> files(filenames.length(),[&filenames](size_t k)
			 *     {
			 * //  Assume Files are move constructible
			 *     return File(filenames[k]);
			 *     });
			 * \endcode
			 *
			 * \note While default constructible is not a requirement for this
			 * constructor, it requires that the objects created in the array
			 * are move constructible, which is equivalent to say that there
			 * must exist a "dead" state for the objects.
			 *
			*/
			template<class Initializer>
			explicit ArraySimple(size_t n_elems,Initializer&& initializer);

			/**\brief Copy constructor.
			 *
			 * This is the copy constructor.
			 *
			 * \note This function only works if the individual objects are copy
			 * constructible
			 *
			*/
			ArraySimple(const ArraySimple& a);

			/**\brief Copy copy assignment operator.
			 *
			 * This is the copy assignment operator.
			 *
			 * \note This function only works if the individual objects are copy
			 * constructible
			 *
			*/
			ArraySimple& operator=(const ArraySimple& a)
				{
				assert( &a != this);
				ArraySimple<T> temp(a);
				*this=std::move(temp);
				return *this;
				}

			/**\brief Move constructor
			 *
			 * This is the move constructor.
			 *
			*/
			ArraySimple(ArraySimple&& a) noexcept
				{
				m_content.x=a.m_content.x;
				a.m_content.x=TwoPointers{0,0};
				}

			/**\brief Move assigment operator
			 *
			 * This is the move assignment operator.
			*/
			ArraySimple& operator=(ArraySimple&& a) noexcept
				{
 				assert(&a != this);
				std::swap(m_content.x,a.m_content.x);
				return *this;
				}

			/**\brief Returns a pointer to the beginning of the array
			 *
			 * This function returns a pointer to the beginning of the array.
			 *
			*/
			T* begin() noexcept
				{return m_content.fields.data;}

			/**\brief Returns a pointer to const to the beginning of the array
			*/
			const T* begin() const noexcept
				{return m_content.fields.data;}

			/**\brief Returns a pointer to the element past the last element
			 *
			 * This function returns a pointer to the element past the last
			 * element.
			*/
			T* end() noexcept
				{return m_content.fields.data+m_content.fields.N;}

			/**\brief Returns a pointer to the element past the last element
			 *
			 * This function returns a pointer to the element past the last
			 * element.
			 *
			*/
			const T* end() const noexcept
				{return m_content.fields.data+m_content.fields.N;}

			/**\brief Returns the length of the array
			 *
			 * This function returns the length of the array.
			 *
			*/
			size_t length() const noexcept
				{return m_content.fields.N;}

			/**\brief Array element access.
			 *
			 * This function returns the element at position <var>k</var>
			 *
			 * \warning The parameter <var>k</var> has to be less than the value returned
			 * by length()
			 *
			*/
			const T& operator[](size_t k) const noexcept
				{
				assert(k<m_content.fields.N);
				return m_content.fields.data[k];
				}

			/**\brief Array element access.
			 *
			 * This function returns the element at position <var>k</var>.
			 *
			 * \warning The parameter <var>k</var> has to be less than the value returned
			 * by length()
			 *
			*/
			T& operator[](size_t k) noexcept
				{
				assert(k<m_content.fields.N);
				return m_content.fields.data[k];
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

		private:
#if (__amd64 || __x86_64 || __x86_64__ || __amd64__)
			typedef vec2_t<int64_t> TwoPointers;
#else
			typedef vec2_t<int32_t> TwoPointers;
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
		assert(n_elems!=0);
		auto data=reinterpret_cast<T*>(memoryAllocate(n_elems*sizeof(T)));
		try
			{ArrayInit::create(data,data+n_elems);}
		catch(...)
			{
			memoryFree(data);
			throw;
			}
		m_content.fields.N=n_elems;
		m_content.fields.data=data;
		}

	template<class T>
	template<class Initializer>
	ArraySimple<T>::ArraySimple(size_t n_elems,Initializer&& init)
		{
		assert(n_elems!=0);
		auto data=reinterpret_cast<T*>(memoryAllocate(n_elems*sizeof(T)));
		try
			{ArrayInit::create(data,data+n_elems,init);}
		catch(...)
			{
			memoryFree(data);
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
	ArraySimple<T>::ArraySimple(const ArraySimple& a)
		{
		assert(a.begin()!=0);
		auto data=reinterpret_cast<T*>( memoryAllocate(a.length()*sizeof(T)) );
		try
			{ArrayInit::copy(data,data+a.length(),a.begin());}
		catch(...)
			{memoryFree(data);}
		m_content.fields.N=a.length();
		m_content.fields.data=data;
		}
	}

#endif
