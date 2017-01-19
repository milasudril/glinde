//@	{"targets":[{"name":"window.hpp","type":"include"}]}

#ifndef GLFWMM_WINDOW_HPP
#define GLFWMM_WINDOW_HPP

#include "windowbase.hpp"

namespace GLFWmm
	{
	template<class Callback>
	class Window:public WindowBase
		{
		public:
			Window(float width,float height,const char* title,Callback& cb):
				WindowBase(width,height,title),r_callback(&cb)
					{
					positionChangedRegister(has_member{});
					sizeChangedRegister(has_member{});
					closingRegister(has_member{});
					refreshRegister(has_member{});
					focusChangedRegister(has_member{});
					iconifiedRegister(has_member{});
					framebufferSizeChangedRegister(has_member{});
					}

		private:
			Callback* r_callback;


			static void positionChanged(GLFWwindow* handle,int x,int y)
				{
				auto self=reinterpret_cast<Window*>(userPointerGet(handle));
				self->r_callback->positionChanged(*self,x,y);
				}

			template<class S=Callback, typename member_test<decltype(&S::positionChanged)>::type=0>
			void positionChangedRegister(has_member)
				{glfwSetWindowPosCallback(m_handle,positionChanged);}

			ADVERTISE_MEMBER("positionChanged")	void positionChangedRegister(has_not_member){}



			static void sizeChanged(GLFWwindow* handle,int x,int y)
				{
				auto self=reinterpret_cast<Window*>(userPointerGet(handle));
				self->r_callback->sizeChanged(*self,x,y);
				}

			template<class S=Callback, typename member_test<decltype(&S::sizeChanged)>::type=0>
			void sizeChangedRegister(has_member)
				{glfwSetWindowSizeCallback(m_handle,sizeChanged);}

			ADVERTISE_MEMBER("sizeChanged") void sizeChangedRegister(has_not_member){}



			static void closing(GLFWwindow* handle)
				{
				auto self=reinterpret_cast<Window*>(userPointerGet(handle));
				self->r_callback->closing(*self);
				}

			template<class S=Callback, typename member_test<decltype(&S::closing)>::type=0>
			void closingRegister(has_member)
				{glfwSetWindowCloseCallback(m_handle,closing);}

			ADVERTISE_MEMBER("closing")	void closingRegister(has_not_member){}



			static void refresh(GLFWwindow* handle)
				{
				auto self=reinterpret_cast<Window*>(userPointerGet(handle));
				self->r_callback->refresh(*self);
				}

			template<class S=Callback, typename member_test<decltype(&S::refresh)>::type=0>
			void refreshRegister(has_member)
				{glfwSetWindowRefreshCallback(m_handle,refresh);}

			ADVERTISE_MEMBER("refresh") void refreshRegister(has_not_member){}



			static void focusChanged(GLFWwindow* handle,int status)
				{
				auto self=reinterpret_cast<Window*>(userPointerGet(handle));
				self->r_callback->focusChanged(*self,status);
				}

			template<class S=Callback, typename member_test<decltype(&S::focusChanged)>::type=0>
			void focusChangedRegister(has_member)
				{glfwSetWindowFocusCallback(m_handle,focusChanged);}

			ADVERTISE_MEMBER("focusChanged") void focusChangedRegister(has_not_member){}



			static void iconified(GLFWwindow* handle,int status)
				{
				auto self=reinterpret_cast<Window*>(userPointerGet(handle));
				self->r_callback->iconified(*self,status);
				}

			template<class S=Callback, typename member_test<decltype(&S::iconified)>::type=0>
			void iconifiedRegister(has_member)
				{glfwSetWindowIconifyCallback(m_handle,iconified);}

			ADVERTISE_MEMBER("iconified") void iconifiedRegister(has_not_member){}



			static void framebufferSizeChanged(GLFWwindow* handle,int x,int y)
				{
				auto self=reinterpret_cast<Window*>(userPointerGet(handle));
				self->r_callback->framebufferSizeChanged(*self,x,y);
				}

			template<class S=Callback, typename member_test<decltype(&S::framebufferSizeChanged)>::type=0>
			void framebufferSizeChangedRegister(has_member)
				{glfwSetFramebufferSizeCallback(m_handle,framebufferSizeChanged);}

			ADVERTISE_MEMBER("framebufferSizeChanged") void framebufferSizeChangedRegister(has_not_member){}
		};
	}

#endif
