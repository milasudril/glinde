//@	{"targets":[{"name":"arraysize.hpp","type":"include"}]}

#ifndef ANGLE_ARRAYSIZE_HPP
#define ANGLE_ARRAYSIZE_HPP

#include <GL/glew.h>

namespace Angle
	{
	template<size_t N,class T>
	static constexpr GLuint size(const T (&array)[N])
		{return sizeof(array)/sizeof(T);}
	}

#endif
