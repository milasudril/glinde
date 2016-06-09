#ifdef __WAND__
target[name[strerror.h] type[include]]
dependency[strerror.o]
#endif

#ifndef GLINDE_STRERROR_H
#define GLINDE_STRERROR_H

#include <cstddef>

namespace Glinde
	{
	int strerror(int errcode,char* buffer,size_t length);
	}

#endif
