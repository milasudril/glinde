//@	{"targets":[{"name":"windowbase.hpp","type":"include"}]}

#ifndef GLFWMM_WINDOWBASE_HPP
#define GLFWMM_WINDOWBASE_HPP

#include "session.hpp"
#include "size.hpp"
#include <utility>

namespace GLFWmm
	{
	class WindowBase
		{
		public:
			enum class Action:int
				{
				 PRESS=GLFW_PRESS
				,RELEASE=GLFW_RELEASE
				,REPEAT=GLFW_REPEAT
				};

			static constexpr unsigned int MOD_SHIFT=GLFW_MOD_SHIFT;
			static constexpr unsigned int MOD_CONTROL=GLFW_MOD_CONTROL;
			static constexpr unsigned int MOD_ALT=GLFW_MOD_ALT;
			static constexpr unsigned int MOD_SUPER=GLFW_MOD_SUPER;

			enum class CursorMode:int
				{
				 CURSOR_NORMAL=GLFW_CURSOR_NORMAL
				,CURSOR_HIDDEN=GLFW_CURSOR_HIDDEN
				,CURSOR_DISABLED=GLFW_CURSOR_DISABLED
				};

			enum class KeyMode:int
				{
				 NORMAL=0
				,STICKY=1
				};

			enum class MouseButtonMode:int
				{
				 NORMAL=0
				,STICKY=1
				};

			enum class CursorDirection:int
				{
				 OUT=0
				,IN=1
				};



			typedef GLFWwindow* ContextHandle;

			Size size() const noexcept
				{
				Size ret;
				glfwGetWindowSize(m_handle,&ret.width,&ret.height);
				return ret;
				}

			Size sizeFb() const noexcept
				{
				Size ret;
				glfwGetFramebufferSize(m_handle,&ret.width,&ret.height);
				return ret;
				}

			std::pair<double,double> cursorPos() const noexcept
				{
				std::pair<double,double> ret;
				glfwGetCursorPos(m_handle,&ret.first,&ret.second);
				return ret;
				}

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

			WindowBase& inputMode(CursorMode mode) noexcept
				{
				glfwSetInputMode(m_handle,GLFW_CURSOR,static_cast<int>(mode));
				return *this;
				}

			WindowBase& inputMode(MouseButtonMode mode) noexcept
				{
				glfwSetInputMode(m_handle,GLFW_STICKY_MOUSE_BUTTONS,static_cast<int>(mode));
				return *this;
				}

			WindowBase& inputMode(KeyMode mode) noexcept
				{
				glfwSetInputMode(m_handle,GLFW_STICKY_KEYS,static_cast<int>(mode));
				return *this;
				}


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
