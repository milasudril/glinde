//@	{"targets":[{"name":"errorcode.hpp","type":"include"}]}

#ifndef ANGLE_ERRORCODE_HPP
#define ANGLE_ERRORCODE_HPP

#include <GL/glew.h>
namespace Angle
	{
	class ErrorCode
		{
		public:
			explicit ErrorCode(GLenum code) noexcept:m_code(code)
				{}

		private:
			GLenum m_code;
		};
	}

#endif
