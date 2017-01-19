//@ {
//@	 "targets":[{"name":"session.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"session.o","rel":"implementation"}]
//@	}

#ifndef GLFWMM_SESSION_HPP
#define GLFWMM_SESSION_HPP

#include <GLFW/glfw3.h>
#include <cstdio>

namespace GLFWmm
	{
	class Session
		{
		public:
			Session():Session(error_handler_default)
				{}

			explicit Session(GLFWerrorfun error_handler)
				{
				glfwSetErrorCallback(error_handler);
				glfwInit();
				}

			~Session() noexcept
				{glfwTerminate();}

			const char* versionGet() const noexcept
				{return glfwGetVersionString();}

			void eventsPoll()
				{glfwPollEvents();}

			void waitEvents()
				{glfwWaitEvents();}
 
			static void error_handler_default(int code,const char* message);
		};
	}

#endif