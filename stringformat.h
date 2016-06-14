#ifdef __WAND__
target[name[stringformat.h] type[include]]
dependency[stringformat.o]
#endif

#ifndef GLINDE_STRINGFORMAT_H
#define GLINDE_STRINGFORMAT_H

#include "variant.h"

#include <cstddef>

namespace Glinde
	{
	template<class T>
	class Range;

	void format(const Range<char>& buffer,const char* format
		,const Range<const Variant>& args);
	}

#endif
