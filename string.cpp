#ifdef __WAND__
target[name[string.o] type[object]]
#endif

#include "string.h"
#include "debug.h"
#include "errormessage.h"
#include <cstring>
#include <cstdlib>
#include <limits>

using namespace Glinie;

String& String::appendImpl(const char* c_str)
	{
	ArrayDynamic<char>::truncate();
	while(*c_str != '\0')
		{
		ArrayDynamic<char>::append(*c_str);
		++c_str;
		}
	ArrayDynamic<char>::append('\0');
	return *this;
	}

bool Glinie::operator==(const String& a,const char* b) noexcept
	{
	if(a.length()==0 && *b!='\0')
		{return 0;}
	return strcmp(a.begin(),b)==0;
	}

bool Glinie::operator==(const String& a,const String& b) noexcept
	{
	if(a.length()!=b.length())
		{return 0;}
	return a==b.begin();
	}

String& String::truncate(char ch) noexcept
	{
	if(length()==0)
		{return *this;}
	auto ptr_begin=begin();
	auto ptr_end=end();
	while(ptr_end!=ptr_begin)
		{
		--ptr_end;
		if(*ptr_end==ch)
			{break;}
		}
	auto N=static_cast<uint32_t>( end() - ptr_end );
	ArrayDynamic<char>::truncate(N);
	(*end())='\0';
	return *this;
	}

const char* String::beginAfter(char ch) const noexcept
	{
	auto ptr_begin=begin();
	auto ptr_end=end();
	while(ptr_begin!=ptr_end)
		{
		if(*ptr_begin==ch)
			{
			++ptr_begin;
			return (ptr_begin!=ptr_end)? ptr_begin: begin();
			}
		++ptr_begin;
		}
	return ptr_begin;
	}

char* String::beginAfter(char ch) noexcept
	{
	auto ptr_begin=begin();
	auto ptr_end=end();
	while(ptr_begin!=ptr_end)
		{
		if(*ptr_begin==ch)
			{
			++ptr_begin;
			return (ptr_begin!=ptr_end)? ptr_begin: begin();
			}
		++ptr_begin;
		}
	return begin();
	}
