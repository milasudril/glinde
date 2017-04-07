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
//@	{"targets":[{"name":"arrayfixed.hpp","type":"include"}]}
/** \file arrayfixed.hpp \brief Definition of ArrayFixed
*/

#ifndef GLINDE_ARRAYFIXED_HPP
#define GLINDE_ARRAYFIXED_HPP

#include "range.hpp"
#include <cstddef>
#include <initializer_list>
#include <utility>

namespace Glinde
	{
	/**\brief Class representing an array allocated at compile-time
	 *
	 * This class describes statically allocated arrays.
	 *
	 * \param T is the element type
	 * \param N is the number of elements
	 *
	*/
	template<class T,size_t N>
	class ArrayFixed
		{
		public:

			/**\brief Default constructor
			 */
			constexpr ArrayFixed() noexcept:data{}
				{}

			/**\brief Element initialize constructor
			 *
			 * This constructor copies the initializer list to the array.
			 *
			 */
			template<typename... U>
			constexpr explicit ArrayFixed(const T& x,const U&... values):data{x,values...}
				{}

			/**\brief Element initialize constructor
			 *
			 * This constructor copies the initializer list to the array.
			 *
			 */
			template<typename... U>
			constexpr explicit ArrayFixed(T&& x,U&&... values):data{std::move(x),std::move(values)...}
				{}


			constexpr ArrayFixed(const ArrayFixed&)=default;


			/**\brief Element access
			 *
			 * This function returns the element at position <var>k</var>
			 *
			 * \warning The parameter <var>k</var> has to be less than the value
			 * returned by length()
			 *
			 */
			constexpr const T& operator[](size_t k) const noexcept
				{
				assert(k<N);
				return data[k];
				}

			/**\brief Element access
			 *
			 * This function returns the element at position <var>k</var>
			 *
			 * \warning The parameter <var>k</var> has to be less than the value
			 * returned by length()
			 *
			 */
			constexpr T& operator[](size_t k) noexcept
				{
				assert(k<N);
				return data[k];
				}

			/**\brief Returns a pointer to the beginning of the array
			 *
			 * This function returns a pointer to the beginning of the array.
			*/
			constexpr const T* begin() const noexcept
				{return data;}

			/**\brief Returns a pointer to the element past the last element
			 *
			 * This function returns a pointer to the element past the last element.
			*/
			constexpr const T* end() const noexcept
				{return data+N;}

			/**\brief Returns a pointer to the beginning of the array
			 *
			 * This function returns a pointer to the beginning of the array.
			*/
			T* begin() noexcept
				{return data;}

			/**\brief Returns a pointer to the element past the last element
			 *
			 * This function returns a pointer to the element past the last element.
			 */
			T* end() noexcept
				{return data+N;}

			/**\brief Returns the length of the array
			 *
			 * This function returns the length of the array.
			 *
			*/
			static constexpr size_t length() noexcept
				{return N;}

			/**\brief Array range
			 *
			 * This function returns a Range representing the content of the array.
			 *
			 */
			constexpr operator Range<const T>() const noexcept
				{return Range<const T>{data,N};}

			/**\brief Array range
			 *
			 * This function returns a Range representing the content of the array.
			 *
			 */
			operator Range<T>() noexcept
				{return Range<T>{data,N};}

			template<size_t k>
			constexpr T& get() noexcept
				{
				static_assert(k<N,"");
				return data[k];
				}

			template<size_t k>
			constexpr const T& get() const noexcept
				{
				static_assert(k<N,"");
				return data[k];
				}

		private:
			T data[N];
		};

	template<class T>
	class ArrayFixed<T,0>
		{};
	}

#endif
