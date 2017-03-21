//@	{"targets":[{"name":"debug.o","type":"object"}]}

#ifndef NDEBUG

#include "debug.hpp"
#include "stringformat.hpp"
#include "variant.hpp"
#include <cstdio>

void Glinde::logWriteDebug(const char* file,uint64_t line
	,const char* format_string
	,const std::initializer_list<Variant>& args) noexcept
	{
	char buffer_a[4096];
	format(Range<char>(buffer_a,sizeof(buffer_a)/sizeof(buffer_a[0]))
		,format_string,args);

	char buffer_b[4096];
	format(Range<char>(buffer_b,sizeof(buffer_b)/sizeof(buffer_b[0]))
		,"(DEBUG) #0;:#1;: #2;\n",{file,line,buffer_a});
	fputs(buffer_b,stderr);
	}

#endif
