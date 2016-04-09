#ifdef __WAND__
target[name[stringkey.o] type[object]]
#endif

#include "stringkey.h"

using namespace Glinda;

static uint64_t hashFnv(const char *s)
	{
	uint64_t hashval = 0xcbf29ce484222325llu;
	auto p=reinterpret_cast<const unsigned char*>(s);
	while(*p!='\0')
		{
		hashval^=*p;
		hashval*=0x100000001b3;
		++p;
		}
	return hashval;
	}

Stringkey::Stringkey(const char* s) noexcept:m_value(hashFnv(s))
	{}
