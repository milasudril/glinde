#ifdef __WAND__
target[name[memoryalloc.o] type[object] platform[;GNU/Linux]]
#endif

#include "memoryalloc.h"
#include "errormessage.h"
#include <cstdlib>

using namespace Glinda;

void* Glinda::memoryAllocate(size_t N)
	{
	void* ret;
	if(posix_memalign(&ret,64,N)!=0)
		{
		throw ErrorMessage("It was not possible to allocate %zu bytes of memory"
			,N);
		}
	return ret;
	}

void Glinda::memoryFree(void* buffer)
	{
	free(buffer);
	}
