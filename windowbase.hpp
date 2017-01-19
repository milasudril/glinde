//@	{"targets":[{"name":"windowbase.hpp","type":"include"}]}

#ifndef GLFWMM_WINDOWBASE_HPP
#define GLFWMM_WINDOWBASE_HPP

#include "session.hpp"

namespace GLFWmm
	{
	class WindowBase
		{
		public:
			typedef GLFWwindow* ContextHandle;

			bool shouldClose() const noexcept
				{return glfwWindowShouldClose(m_handle);}

			GLFWwindow* contextCapture()
				{
				auto ret=glfwGetCurrentContext();
				glfwMakeContextCurrent(m_handle);
				return ret;
				}

			static void contextRelease(GLFWwindow* handle)
				{glfwMakeContextCurrent(handle);}

			void buffersSwap() noexcept
				{glfwSwapBuffers(m_handle);}


		protected:
			WindowBase(float width,float height,const char* title,Session&)
				{
				auto video_mode=glfwGetVideoMode(glfwGetPrimaryMonitor());
				m_handle=glfwCreateWindow(width*video_mode->width
					,height*video_mode->height,title,NULL,NULL);
				glfwSetWindowUserPointer(m_handle,this);
				}

			~WindowBase()
				{glfwDestroyWindow(m_handle);}

			static void* userPointerGet(GLFWwindow* handle) noexcept
				{return glfwGetWindowUserPointer(handle);}

			GLFWwindow* m_handle;
		};
	}
#endif
