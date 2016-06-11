#ifdef __WAND__
target[name[message.o] type[object]]
#endif

#include "message.h"
#include "memoryalloc.h"
#include <cstring>

using namespace Glinde;

void Message::copyAlloc(const void* src,size_t N)
	{
	if(src!=nullptr)
		{
		auto mem=memoryAllocate(N);
		memcpy(mem,src,N);
		m_content.data.length=N;
		m_content.data.data=mem;
		}
	}

Message::~Message()
	{
	memoryFree(m_content.data.data);
	}
