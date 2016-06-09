#ifdef __WAND__
target[name[memoryreader.o] type[object]]
#endif

#include "memoryreader.h"
#include <algorithm>
#include <cstring>

using namespace Glinde;

size_t MemoryReader::read(void* buffer,size_t N)
	{
	auto n_read=std::min(N,m_content.fields.N);

	memcpy(buffer,m_content.fields.data,n_read);

	m_content.fields.data+=n_read;
	m_content.fields.N-=n_read;

	return n_read;
	}
