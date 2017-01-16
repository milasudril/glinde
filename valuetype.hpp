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

	constexpr size_t size(ValueType type) noexcept
		{
		switch(type)
			{
			case ValueType::BYTE:
				return sizeof(GLbyte);
			case ValueType::SHORT:
				return sizeof(GLshort);
			case ValueType::INT:
				return sizeof(GLint);
			case ValueType::FLOAT:
				return sizeof(GLfloat);
			case ValueType::DOUBLE:
				return sizeof(GLdouble);
			case ValueType::UNSIGNED_BYTE:
				return sizeof(GLbyte);
			case ValueType::UNSIGNED_SHORT:
				return sizeof(GLshort);
			case ValueType::UNSIGNED_INT:
				return sizeof(GLint);
			default:
				return 1;
			}
		}

	template<ValueType type>
	struct TypeGet{};

	template<class T>
	struct ConstantGet{};



	template<>
	struct TypeGet<ValueType::BYTE>
		{typedef GLbyte type;};

	template<>
	struct ConstantGet<GLbyte>
		{static constexpr auto value=ValueType::BYTE;};



	template<>
	struct TypeGet<ValueType::SHORT>
		{typedef GLshort type;};

	template<>
	struct ConstantGet<GLshort>
		{static constexpr auto value=ValueType::SHORT;};



	template<>
	struct TypeGet<ValueType::INT>
		{typedef GLint type;};

	template<>
	struct ConstantGet<GLint>
		{static constexpr auto value=ValueType::INT;};



	template<>
	struct TypeGet<ValueType::FLOAT>
		{typedef GLfloat type;};

	template<>
	struct ConstantGet<GLfloat>
		{static constexpr auto value=ValueType::FLOAT;};



	template<>
	struct TypeGet<ValueType::DOUBLE>
		{typedef GLdouble type;};

	template<>
	struct ConstantGet<GLdouble>
		{static constexpr auto value=ValueType::DOUBLE;};



	template<>
	struct TypeGet<ValueType::UNSIGNED_BYTE>
		{typedef GLubyte type;};

	template<>
	struct ConstantGet<GLubyte>
		{static constexpr auto value=ValueType::UNSIGNED_BYTE;};



	template<>
	struct TypeGet<ValueType::UNSIGNED_SHORT>
		{typedef GLushort type;};

	template<>
	struct ConstantGet<GLushort>
		{static constexpr auto value=ValueType::UNSIGNED_SHORT;};



	template<>
	struct TypeGet<ValueType::UNSIGNED_INT>
		{typedef GLuint type;};

	template<>
	struct ConstantGet<GLuint>
		{static constexpr auto value=ValueType::UNSIGNED_INT;};
		



	constexpr GLenum native_type(ValueType x) noexcept
		{return static_cast<GLenum>(x);}
	}

#endif
