#ifdef __WAND__
target[name[memoryalloc.h] type[include]]
dependency[memoryalloc.o]
#endif

/**\file memoryalloc.h \brief Memory allocation routines.
 */

#ifndef GLINDA_ALLOC_H
#define GLINDA_ALLOC_H

#include <cstddef>

namespace Glinda
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
	}

#endif
