#ifdef __WAND__
target[name[vectortype.h] type[include]]
#endif

#ifndef GLINDA_VECTORTYPE_H
#define GLINDA_VECTORTYPE_H

namespace Glinda
	{
	template<class T>
	using vec4_t __attribute__ ((vector_size(4*sizeof(T))))=T;
	}

#endif
