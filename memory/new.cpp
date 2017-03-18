//@	{"targets":[{"name":"new.o","type":"object"}]}

#include "memoryalloc.hpp"

using namespace Glinde;

void* operator new(size_t n_bytes)
	{return memoryAllocate(n_bytes);}

void operator delete(void* buffer)
	{memoryFree(buffer);}

void* operator new[](size_t n_bytes)
	{return memoryAllocate(n_bytes);}

void operator delete[](void* buffer)
	{memoryFree(buffer);}

void operator delete(void* buffer,size_t n)
	{memoryFree(buffer);}

void operator delete[](void* buffer,size_t n)
	{memoryFree(buffer);}
