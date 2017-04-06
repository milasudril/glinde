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
//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"vectortype.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
/**\file vectortype.h \brief Defines data types for enabling vectorized code.
 */

#ifndef GLINDE_VECTORTYPE_H
#define GLINDE_VECTORTYPE_H

#ifdef DOXYGEN
#define __attribute__(x)
#define vector_type(Type) vector_type<Type>
#else
#define vector_type(Type) Type
#endif

namespace Glinde
	{
#ifdef DOXYGEN
	/**\brief class used to define types that uses vectorized instruction sets
	 * like MMX and SSE.
	 *
	 * This class is used to define types that uses vectorized instruction sets
	 * like MMX and SSE. With such instruction sets, the compiler may emit
	 * instructions that operates on multiple data at ones.
	 *
	 *  \param T is the component type of the vector. `T` must be a primitive type
	 *           supported by the targeted instruction set.
	 *
	 *  \warning In order for these types to work properly, the vector_type object
	 * must be aligned properly. To avoid alignment problems, use memoryAllocate(size_t)
	 * and memoryFree(void*) in favour of malloc and free.
	 */
	template<class T> class vector_type;
#endif

	/**\brief Vector type with four components.
	 *
	 * This type represents a vector with four components. For example,
	 * \code
	 * Glinde::vec4_t<int> a{1,2,3,4};
	 * Glinde::vec4_t<int> b{4,3,2,1};
	 * auto c=a+b;
	 * Glinde::logWrite(Log::MessageType::INFORMATION,"c=[%d,%d,%d,%d]"
	 *     ,c[0],c[1],c[2],c[3]);
	 * \endcode
	 * will print
	 *
	 *     (i) c=[5,5,5,5]
	 *
	 * to the console.
	 */
	template<class T>
	using vec4_t __attribute__ ((vector_size(4*sizeof(T))))=vector_type(T);

	/**\brief Vector type with two components.
	 *
	 * This type represents a vector with four components. For example,
	 * \code
	 * Glinde::vec2_t<int> a{1,2};
	 * Glinde::vec2_t<int> b{4,3};
	 * auto c=a+b;
	 * Glinde::logWrite(Log::MessageType::INFORMATION,"c=[%d,%d]",c[0],c[1]);
	 * \endcode
	 * will print
	 *
	 *     (i) c=[5,5]
	 *
	 * to the console.
	 */
	template<class T>
	using vec2_t __attribute__ ((vector_size(2*sizeof(T))))=vector_type(T);
	}

#endif
