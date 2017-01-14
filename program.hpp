//@	{"targets":[{"name":"program.hpp","type":"include"}]}

#ifndef ANGLE_PROGRAM_HPP
#define ANGLE_PROGRAM_HPP

#include "shader.hpp"

namespace Angle
	{
	class Program
		{
		public:
			template<class ... T>
			Program(Shader& shader,T&...shaders)
				{
				int handles[sizeof...(T)+1];
				fill(handles,shader,shaders...);
				for(size_t k=0;k<sizeof...(T)+1;++k)
					{printf("%d\n",handles[k]);}
				}

			template<class ... T>
			Program(Shader&& shader,T&&...shaders)
				{
				int handles[sizeof...(T)+1];
				fill(handles,shader,shaders...);
				for(size_t k=0;k<sizeof...(T)+1;++k)
					{printf("%d\n",handles[k]);}
				}

		private:
      		static constexpr void fill(int* handles)
				{}
			template<class ...T>
			static constexpr void fill(int* handles,Shader& shader,T&...shaders)
				{
				handles[0]=shader.handle();
				fill(handles+1,shaders...);
				}
			int m_handle;
		};

/*	template<class ExceptionHandler>
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
		}*/
	}

#endif
