#ifdef __WAND__
target[name[arrayfixed.h] type[include]]
#endif

/** \file arrayfixed.h \brief Definition of ArrayFixed
*/

#ifndef GLINDE_ARRAYFIXED_H
#define GLINDE_ARRAYFIXED_H

#include "range.h"
#include <cstddef>
#include <initializer_list>

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
			constexpr ArrayFixed(){}

			/**\brief Element initialize constructor
			 *
			 * This constructor copies the initializer list to the array.
			 *
			 */
			template<typename... U>
			constexpr explicit ArrayFixed(U... values):data{values...}
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
			constexpr const T& operator[](size_t k) const
				{return data[k];}

			/**\brief Element access
			 *
			 * This function returns the element at position <var>k</var>
			 *
			 * \warning The parameter <var>k</var> has to be less than the value
			 * returned by length()
			 *
			 */
			T& operator[](size_t k)
				{return data[k];}

			/**\brief Returns a pointer to the beginning of the array
			 *
			 * This function returns a pointer to the beginning of the array.
			*/
			constexpr const T* begin() const
				{return data;}

			/**\brief Returns a pointer to the element past the last element
			 *
			 * This function returns a pointer to the element past the last element.
			*/
			constexpr const T* end() const
				{return data+N;}

			/**\brief Returns a pointer to the beginning of the array
			 *
			 * This function returns a pointer to the beginning of the array.
			*/
			T* begin()
				{return data;}

			/**\brief Returns a pointer to the element past the last element
			 *
			 * This function returns a pointer to the element past the last element.
			 */
			T* end()
				{return data+N;}

			/**\brief Returns the length of the array
			 *
			 * This function returns the length of the array.
			 *
			*/
			static constexpr size_t length()
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

		private:
			T data[N];
		};

	template<class T>
	class ArrayFixed<T,0>
		{};
	}

#endif
