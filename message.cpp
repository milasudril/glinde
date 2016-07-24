//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"message.o",
//@	            "type":"object"
//@	        }
//@	    ]
//@	}
#include "message.h"
#include "memoryalloc.h"
#include "narrow_cast.h"
#include <cstring>

using namespace Glinde;

Message::Processor Message::s_noop;

void Message::copyAlloc(const void* src,size_t N)
	{
	if(src!=nullptr)
		{
		auto mem=memoryAllocate(N);
		memcpy(mem,src,N);
		m_msg.content.pointer=mem;
		m_msg.content.params.dwords[0]=narrow_cast<uint32_t>(N);
		}
	}

void Message::dataFree() noexcept
	{
	if(m_msg.content.handler&OBJECT_OWNED)
		{
		memoryFree(m_msg.content.pointer);
		}
	}
