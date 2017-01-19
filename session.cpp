//@ {
//@	 "targets":[{"name":"session.o","type":"object","pkgconfig_libs":["glfw3"]}]
//@	}

#include "session.hpp"
#include <cstdlib>
#include <cstdio>

using namespace GLFWmm;

void Session::error_handler_default(int code,const char* message)
	{
	fprintf(stderr,"%s (%x)\n",message,code);
	abort();
	}
