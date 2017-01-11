//@	{"targets":[{"name":"valuetype.hpp","type":"include"}]}

#ifndef ANGLE_VALUETYPE_HPP
#define ANGLE_VALUETYPE_HPP

#include <GL/glew.h>

namespace Angle
	{
	enum class ValueType:GLenum
		{
		 BYTE=GL_BYTE
		,SHORT=GL_SHORT
		,INT=GL_INT
		,FIXED=GL_FIXED
		,FLOAT=GL_FLOAT
		,DOUBLE=GL_DOUBLE
		,UNSIGNED_BYTE=GL_UNSIGNED_BYTE
		,UNSIGNED_SHORT=GL_UNSIGNED_SHORT
		,UNSIGNED_INT=GL_UNSIGNED_INT 
		};

	constexpr GLenum native_type(ValueType x) noexcept
		{return static_cast<GLenum>(x);}
	}

#endif
