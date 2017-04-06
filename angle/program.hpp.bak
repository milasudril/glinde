//@	{"targets":[{"name":"program.hpp","type":"include"}]}

#ifndef ANGLE_PROGRAM_HPP
#define ANGLE_PROGRAM_HPP

#include "shader.hpp"

namespace Angle
	{
	class Program
		{
		public:
			template<class T>
			using vec4 __attribute__ ((vector_size(4*sizeof(T))))=T;

			template<class T>
			using vec2 __attribute__ ((vector_size(2*sizeof(T))))=T;


			template<class ... T>
			explicit Program(const Shader& shader,const T&...shaders);

			Program(const Program&)=delete;
			Program& operator=(const Program&)=delete;

			Program(Program&& obj) noexcept:m_handle(obj.m_handle)
				{obj.m_handle=0;}

			Program& operator=(Program&& obj)
				{
				std::swap(obj.m_handle,m_handle);
				return *this;
				}

			~Program() noexcept
				{glDeleteProgram(m_handle);}

			void bind() const noexcept
				{glUseProgram(m_handle);}

			void unbind() const noexcept
				{glUseProgram(0);}


			template<GLint loc>
			Program& uniform(GLint v) noexcept
				{
				glProgramUniform1i(m_handle,loc,v);
				return *this;
				}

			template<GLint loc>
			Program& uniform(GLint v0,GLint v1) noexcept
				{
				glProgramUniform2i(m_handle,loc,v0,v1);
				return *this;
				}

			template<GLint loc>
			Program& uniform(GLint v0,GLint v1,GLint v2) noexcept
				{
				glProgramUniform3i(m_handle,loc,v0,v1,v2);
				return *this;
				}

			template<GLint loc>
			Program& uniform(GLint v0,GLint v1,GLint v2,GLint v3) noexcept
				{
				glProgramUniform4i(m_handle,loc,v0,v1,v2,v3);
				return *this;
				}


			template<GLint loc>
			Program& uniform(float v) noexcept
				{
				glProgramUniform1f(m_handle,loc,v);
				return *this;
				}

			template<GLint loc>
			Program& uniform(float v0,float v1) noexcept
				{
				glProgramUniform2f(m_handle,loc,v0,v1);
				return *this;
				}

			template<GLint loc>
			Program& uniform(float v0,float v1,float v2) noexcept
				{
				glProgramUniform3f(m_handle,loc,v0,v1,v2);
				return *this;
				}

			template<GLint loc>
			Program& uniform(float v0,float v1,float v2,float v3) noexcept
				{
				glProgramUniform4f(m_handle,loc,v0,v1,v2,v3);
				return *this;
				}


		
			template<GLint loc>
			Program& uniform(const GLint* v,GLsizei count) noexcept
				{
				glProgramUniform1iv(m_handle,loc,count,v);
				return *this;
				}

			template<GLint loc>
			Program& uniform(const vec2<GLint>* v,GLsizei count) noexcept
				{
				glProgramUniform2iv(m_handle,loc,count,reinterpret_cast<const GLint*>(v));
				return *this;
				}

			template<GLint loc>
			Program& uniform(const vec4<GLint>* v,GLsizei count) noexcept
				{
				glProgramUniform4iv(m_handle,loc,count,reinterpret_cast<const GLint*>(v));
				return *this;
				}



			template<GLint loc>
			Program& uniform(const float* v,GLsizei count) noexcept
				{
				glProgramUniform1fv(m_handle,loc,count,v);
				return *this;
				}

			template<GLint loc>
			Program& uniform(const vec2<float>* v,GLsizei count) noexcept
				{
				glProgramUniform2fv(m_handle,loc,count,reinterpret_cast<const float*>(v));
				return *this;
				}

			template<GLint loc>
			Program& uniform(const vec4<float>* v,GLsizei count) noexcept
				{
				glProgramUniform4fv(m_handle,loc,count,reinterpret_cast<const float*>(v));
				return *this;
				}




		private:
      		static constexpr void collect(GLuint* handles) noexcept
				{}
			template<class ...T>
			static constexpr void collect(GLuint* handles,const Shader& shader,const T&...shaders) noexcept
				{
				handles[0]=shader.handle();
				collect(handles+1,shaders...);
				}
			GLuint m_handle;
		};

	template<class ... T>
	Program::Program(const Shader& shader,const T&...shaders)
		{
		assert(glCreateProgram!=nullptr);
		m_handle=glCreateProgram();
		GLuint handles[sizeof...(T)+1];
		collect(handles,shader,shaders...);
		for(size_t k=0;k<sizeof...(T)+1;++k)
			{glAttachShader(m_handle,handles[k]);}
		glLinkProgram(m_handle);
		auto status=static_cast<GLint>(0);
		glGetProgramiv(m_handle,GL_LINK_STATUS,&status);
		if(status!=GL_TRUE)
			{
			auto length=static_cast<GLint>(0);
			glGetProgramiv(m_handle,GL_INFO_LOG_LENGTH,&length);
			char message[1024];
			glGetProgramInfoLog(m_handle,std::min(1024,length),NULL,message);
			for(size_t k=0;k<sizeof...(T)+1;++k)
				{glDetachShader(m_handle,handles[k]);}
			glDeleteProgram(m_handle);
			exceptionRaise(Error("It was not possible to link the shader program. ",message));
			}
		}
	}

#endif
