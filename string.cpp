#ifdef __WAND__
target[name[string.o] type[object]]
#endif

#include "string.h"
#include "debug.h"
#include "errormessage.h"
#include <cstring>
#include <cstdlib>
#include <limits>

using namespace Glinda;

#if ( _WIN32 && (__amd64 || __x86_64 || __x86_64__ || __amd64__) )
	#define __builtin_clz(N) __builtin_clzll(N)
#else
	#define __builtin_clz(N) __builtin_clzl(N)
#endif

static size_t allocSize(size_t N)
	{
	return 1<<(8*sizeof(N) - 1 - __builtin_clz(N) + 1);
	}

void String::capacitySet(size_t N)
	{
#ifndef NDEBUG
		{
		auto N_2=allocSize(N);
		GLINDA_DEBUG_PRINT("Allocating string of size %zu (requested size is %zu)",N_2,N);
		assert( N_2 >= N );
		N=N_2;
		}
#else
	auto N_2=allocSize(N);
#endif


	auto content=m_content;

	if(N > std::numeric_limits<uint32_t>::max())
		{throw ErrorMessage("The string does not fit in its container.");}

	auto ptr=static_cast<char*>( realloc(content.data.pointer,N*sizeof(char) ));
	if(ptr==nullptr)
		{
		throw ErrorMessage("It is not possible to allocate more space for this string.");
		}

	content.data.pointer=ptr;
	content.data.capacity=static_cast<uint32_t>(N);
	m_content.x=content.x;
	}

String& String::appendImpl(const char* c_str)
	{
	while(*c_str != '\0')
		{
		append(*c_str);
		++c_str;
		}
	return *this;
	}

String& String::appendImpl(const String& str)
	{
	auto N=str.length() + 1;
	auto N_tot=N+length();
	capacitySet(N_tot);
	memcpy(end(),str.begin(),N);
	m_content.data.length=N_tot;
	return *this;
	}

String::~String()
	{
	free(m_content.data.pointer);
	}
