//@	{
//@	"targets":
//@		[{
//@		 "dependencies":[{"ref":"jemalloc","rel":"external"}]
//@		,"name":"memoryalloc.o","type":"object"
//@		}]
//@	}

#include "memoryalloc.hpp"
#include "../debug.hpp"
#include "../variant.hpp"
#include "../errormessage.hpp"
#include <jemalloc/jemalloc.h>
#include <cstdlib>

using namespace Glinde;

#ifdef __AVX__
static constexpr size_t ALIGNMENT=32;
#else
static constexpr size_t ALIGNMENT=16;
#endif

#ifndef NDEBUG
static size_t malloc_count=0;
#define GLINDE_REGISTER_MALLOC_DTOR []() \
	{ \
	if(malloc_count==0) \
		{ \
		atexit([]() \
			{ \
			if( malloc_count!=0 ) \
				{ \
				GLINDE_DEBUG_PRINT("Leak detected: #0; objects are left on the heap.",{malloc_count}); \
				abort(); \
				} \
			}); \
		} \
	}

#define GLINDE_MALLOC_INCREMENT [](){++malloc_count;}
#define GLINDE_MALLOC_DECREMENT []()\
	{\
	if(malloc_count==0) \
		{ \
		GLINDE_DEBUG_PRINT("Double free detected");\
		abort(); \
		} \
	--malloc_count; \
	}
#else
#define GLINDE_REGISTER_MALLOC_DTOR
#define GLINDE_MALLOC_INCREMENT
#define GLINDE_MALLOC_DECREMENT
#endif

void* Glinde::memoryAllocate(size_t N)
	{
	GLINDE_REGISTER_MALLOC_DTOR();
	void* ret=mallocx(N,MALLOCX_ALIGN(ALIGNMENT));
	if(ret==nullptr)
		{
		throw ErrorMessage("It was not possible to allocate #0; bytes of memory"
			,{N});
		}
	GLINDE_MALLOC_INCREMENT();
	return ret;
	}

void Glinde::memoryFree(void* buffer)
	{
	if(buffer!=nullptr)
		{
		GLINDE_MALLOC_DECREMENT();
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
