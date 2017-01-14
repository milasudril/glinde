//@	{"targets":[{"name":"shader.hpp","type":"include"}]}

#ifndef ANGLE_SHADER_HPP
#define ANGLE_SHADER_HPP

#include <GL/glew.h>
#include <cassert>
#include <algorithm>
#include <cstring>

namespace Angle
	{
	enum class ShaderType:GLenum
		{
		 COMPUTE_SHADER=GL_COMPUTE_SHADER
		,VERTEX_SHADER=GL_VERTEX_SHADER
		,TESS_CONTROL_SHADER=GL_TESS_CONTROL_SHADER
		,TESS_EVALUATION_SHADER=GL_TESS_EVALUATION_SHADER
		,GEOMETRY_SHADER=GL_GEOMETRY_SHADER
		,FRAGMENT_SHADER=GL_FRAGMENT_SHADER
		};

	static constexpr GLenum native_type(ShaderType type) noexcept
		{return static_cast<GLenum>(type);}

	static constexpr const char* name(ShaderType type) noexcept
		{
		switch(type)
			{
			case ShaderType::COMPUTE_SHADER:
				return "compute shader";

			case ShaderType::VERTEX_SHADER:
				return "vertex shader";

			case ShaderType::TESS_CONTROL_SHADER:
				return "tesselation control shader";

			case ShaderType::TESS_EVALUATION_SHADER:
				return "tesselation evaluation shader";

			case ShaderType::GEOMETRY_SHADER:
				return "geometry shader";

			case ShaderType::FRAGMENT_SHADER:
				return "fragment shader";
			}
		}

	class Shader
		{
		public:
			template<class ExceptionHandler>
			explicit Shader(const char* source,ShaderType type,ExceptionHandler&& eh);

			~Shader() noexcept
				{glDeleteShader(m_handle);}

			GLuint handle() noexcept
				{return m_handle;}

		private:
			GLuint m_handle;
		};

	template<class ExceptionHandler>
	Shader::Shader(const char* source,ShaderType type,ExceptionHandler&& eh)
		{
		assert(glCreateShader!=nullptr);
		m_handle=glCreateShader(native_type(type));
		assert(m_handle!=0);

		auto length=static_cast<GLint>(strlen(source));
		glShaderSource(m_handle,1,&source,&length);
		glCompileShader(m_handle);
		auto status=static_cast<GLint>(0);
		glGetShaderiv(m_handle,GL_COMPILE_STATUS,&status);
		if(status!=GL_TRUE)
			{
			auto length=static_cast<GLint>(0);
			glGetShaderiv(m_handle,GL_INFO_LOG_LENGTH,&length);
			char message[1024];
			glGetShaderInfoLog(m_handle,std::min(1024,length),NULL,message);
			glDeleteShader(m_handle);
			eh(type,message);
			m_handle=0;
			}
		}
	}

#endif
