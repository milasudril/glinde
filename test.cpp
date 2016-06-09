#include "string.h"
#include <cstdio>

void foo(const char* bar)
	{
	puts(bar);
	}

void foo(const Glinde::String& bar)
	{
	puts(bar.begin());
	}


void call_foo()
	{
	foo("Hello, World");
	foo(Glinde::String("Hello, World"));
	}
