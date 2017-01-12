//@	{"targets":[{"name":"shader.hpp","type":"include"}]}

#ifndef ANGLE_SHADER_HPP
#define ANGLE_SHADER_HPP

#include <GL/glew.h>
#include <cassert>

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

	constexpr GLenum native_type(ShaderType type) noexcept
		{return static_cast<GLenum>(type);}

	class Shader
		{
		public:
			template<class ExceptionHandler>
			explicit Shader(const char* source,ShaderType type,ExceptionHandler&& eh)
				{
				assert(glCreateShader!=nullptr);
				m_handle=glCreateShader(native_type(type));
				assert(m_handle!=0);

				
				}

			~Shader() noexcept;
				{glDeleteShader(m_handle);}
		private:
			GLuint m_handle;
		};
	}

#endif
