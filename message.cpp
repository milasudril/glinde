#ifdef __WAND__
target[name[message.o] type[object]]
#endif

#include "message.h"
#include "memoryalloc.h"
#include <cstring>

using namespace Glinde;

Message::Processor Message::s_noop;

void Message::copyAlloc(const void* src,size_t N)
	{
	if(src!=nullptr)
		{
		auto mem=memoryAllocate(N);
		memcpy(mem,src,N);
		m_msg.content.data.intptrs[0]=reinterpret_cast<uintptr_t>( mem );
		m_msg.content.data.intptrs[1]=N;
		}
	}

void Message::dataFree() noexcept
	{
	if(m_msg.content.handler&OBJECT_OWNED)
		{memoryFree(reinterpret_cast<void*>(m_msg.content.data.intptrs[0]));}
	}
