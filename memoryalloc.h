#ifdef __WAND__
target[name[memoryalloc.h] type[include]]
dependency[memoryalloc.o]
#endif

#ifndef GLINDA_ALLOC_H
#define GLINDA_ALLOC_H

#include <cstddef>

namespace Glinda
	{
	void* memoryAllocate(size_t N);
	void memoryFree(void* buffer);
	}

#endif
