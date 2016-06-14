#ifdef __WAND__
target[name[memoryalloc.o] type[object] dependency[jemalloc;external]]
#endif

#define NDEBUG

#include "memoryalloc.h"
#include "errormessage.h"
#include "debug.h"
#include "variant.h"
#include <jemalloc/jemalloc.h>

using namespace Glinde;

void* Glinde::memoryAllocate(size_t N)
	{
	void* ret=mallocx(N,MALLOCX_ALIGN(64));
	if(ret==nullptr)
		{
		throw ErrorMessage("It was not possible to allocate %zu bytes of memory"
			,N);
		}
	GLINDE_DEBUG_PRINT("Allocated #0; bytes at #1;",N,ret);
	return ret;
	}

void Glinde::memoryFree(void* buffer)
	{
	if(buffer!=nullptr)
		{
		GLINDE_DEBUG_PRINT("DeAllocating #0;",buffer);
		dallocx(buffer,MALLOCX_ALIGN(64));
		}
	}

void* Glinde::memoryRealloc(void* buffer,size_t N)
	{
	if(buffer==nullptr)
		{return memoryAllocate(N);}
	void* ret=rallocx(buffer,N,MALLOCX_ALIGN(64));
	if(ret==nullptr)
		{
		throw ErrorMessage("It was not possible to resize the block at %p to %zu bytes"
			,buffer,N);
		}
	return ret;
	}
