//@	{
//@	"targets":
//@		[{
//@		 "dependencies":[{"ref":"jemalloc","rel":"external"}]
//@		,"name":"memoryalloc.o","type":"object"
//@		,"cxxoptions":{"cflags_extra":["DNDEBUG"]}
//@		}]
//@	}

#include "memoryalloc.hpp"
#include "errormessage.hpp"
#include "debug.hpp"
#include "variant.hpp"
#include <jemalloc/jemalloc.h>

using namespace Glinde;

#ifdef __AVX__
static constexpr size_t ALIGNMENT=32;
#else
static constexpr size_t ALIGNMENT=16;
#endif

void* Glinde::memoryAllocate(size_t N)
	{
	void* ret=mallocx(N,MALLOCX_ALIGN(ALIGNMENT));
	if(ret==nullptr)
		{
		throw ErrorMessage("It was not possible to allocate #0; bytes of memory"
			,{N});
		}
	GLINDE_DEBUG_PRINT("Allocated #0; bytes at #1;",N,ret);
	return ret;
	}

void Glinde::memoryFree(void* buffer)
	{
	if(buffer!=nullptr)
		{
		GLINDE_DEBUG_PRINT("DeAllocating #0;",buffer);
		dallocx(buffer,MALLOCX_ALIGN(ALIGNMENT));
		}
	}

void* Glinde::memoryRealloc(void* buffer,size_t N)
	{
	if(buffer==nullptr)
		{return memoryAllocate(N);}
	void* ret=rallocx(buffer,N,MALLOCX_ALIGN(ALIGNMENT));
	if(ret==nullptr)
		{
		throw ErrorMessage("It was not possible to resize the block at #0; to #1; bytes"
			,{buffer,N});
		}
	return ret;
	}
