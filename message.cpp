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
		m_content.data.r_object=mem;
		m_content.data.m_content.sizes[0]=N;
		}
	}

void Message::dataFree() noexcept
	{
	if((m_content.data.r_object!=nullptr) && (m_content.data.m_handler&OBJECT_OWNED))
		{memoryFree(m_content.data.r_object);}
	}

void Message::no_op(const Message& data)
	{}
