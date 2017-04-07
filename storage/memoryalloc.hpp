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
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"memoryalloc.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"memoryalloc.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
/**\file memoryalloc.h \brief Memory allocation routines.
 *
 * The memory allocation routines declared in this file are SSE safe.
 *
 * \note These functions are implement through the `jemalloc` library by
 * Jason Evans. It has been released under the two-clause BSD license.
 *
 */

#ifndef GLINDE_ALLOC_H
#define GLINDE_ALLOC_H

#include <cstddef>

namespace Glinde
	{
	/**\brief Allocates N bytes from the heap.
	 *
	 * This function allocates N bytes from the heap. If the function fails
	 * to allocate memory, an ErrorMessage is thrown.
	 *
	 * \note The pointer returned is properly aligned so vectorized instructions
	 * can be used.
	 *
	 * \warning Do not use `free` to free the allocated memory. Use memoryFree(void*)
	 * instead.
	*/
	void* memoryAllocate(size_t N);

	/**\brief Frees memory allocated by memoryAllocate(size_t).
	 *
	 * This function frees memory allocated by memoryAllocate(size_t).
	 *
	 * \warning Do not pass a pointer allocated by `malloc` to this function.
	 *
	 */
	void memoryFree(void* buffer);


	/**\brief Reallocates memory allocated by memoryAllocate(size_t).
	 *
	 * This function reallocates memory allocated by memoryAllocate(size_t).
	 *
	 * \warning Do not pass a pointer allocated by `malloc` to this function.
	 *
	 */
	void* memoryRealloc(void* buffer,size_t N);
	}

#endif
