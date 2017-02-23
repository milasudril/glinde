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
			Window(float width,float height,const char* title,Callback& cb,Session& session):
				WindowBase(width,height,title,session),r_callback(&cb)
					{
					positionChangedRegister(has_member{});
					sizeChangedRegister(has_member{});
					closingRegister(has_member{});
					refreshRegister(has_member{});
					focusChangedRegister(has_member{});
					iconifiedRegister(has_member{});
					framebufferSizeChangedRegister(has_member{});

					mouseButtonRegister(has_member{});
					cursorEnterRegister(has_member{});
					cursorPosRegister(has_member{});
					dropRegister(has_member{});
					scrollRegister(has_member{});

					keyRegister(has_member{});
					codepointRegister(has_member{});
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



			static void mouseButton(GLFWwindow* handle,int button,int action,int mods)
				{
				auto self=reinterpret_cast<Window*>(userPointerGet(handle));
				self->r_callback->mouseButton(*self,button,static_cast<Action>(action)
					,static_cast<unsigned int>(mods));
				}

			template<class S=Callback, typename member_test<decltype(&S::mouseButton)>::type=0>
			void mouseButtonRegister(has_member)
				{glfwSetMouseButtonCallback(m_handle,mouseButton);}

			ADVERTISE_MEMBER("mouseButton") void mouseButtonRegister(has_not_member){}


			
			static void cursorEnter(GLFWwindow* handle,int direction)
				{
				auto self=reinterpret_cast<Window*>(userPointerGet(handle));
				self->r_callback->cursorEnter(*self,static_cast<CursorDirection>(direction));
				}

			template<class S=Callback, typename member_test<decltype(&S::cursorEnter)>::type=0>
			void cursorEnterRegister(has_member)
				{glfwSetCursorEnterCallback(m_handle,cursorEnter);}

			ADVERTISE_MEMBER("cursorEnter") void cursorEnterRegister(has_not_member){}



			static void cursorPos(GLFWwindow* handle,double x,double y)
				{
				auto self=reinterpret_cast<Window*>(userPointerGet(handle));
				self->r_callback->cursorPos(*self,x,y);
				}

			template<class S=Callback, typename member_test<decltype(&S::cursorPos)>::type=0>
			void cursorPosRegister(has_member)
				{glfwSetCursorPosCallback(m_handle,cursorPos);}

			ADVERTISE_MEMBER("cursorPos") void cursorPosRegister(has_not_member){}



			static void drop(GLFWwindow* handle,int count,const char** files)
				{
				auto self=reinterpret_cast<Window*>(userPointerGet(handle));
				self->r_callback->drop(*self,count,files);
				}

			template<class S=Callback, typename member_test<decltype(&S::drop)>::type=0>
			void dropRegister(has_member)
				{glfwSetDropCallback(m_handle,drop);}

			ADVERTISE_MEMBER("drop") void dropRegister(has_not_member){}



			static void scroll(GLFWwindow* handle,double x,double y)
				{
				auto self=reinterpret_cast<Window*>(userPointerGet(handle));
				self->r_callback->scroll(*self,x,y);
				}

			template<class S=Callback, typename member_test<decltype(&S::scroll)>::type=0>
			void scrollRegister(has_member)
				{glfwSetScrollCallback(m_handle,scroll);}

			ADVERTISE_MEMBER("scroll") void scrollRegister(has_not_member){}



			static void key(GLFWwindow* handle,int key,int scancode,int action,int modifiers)
				{
				auto self=reinterpret_cast<Window*>(userPointerGet(handle));
				self->r_callback->key(*self,scancode,static_cast<Action>(action)
					,static_cast<unsigned int>(modifiers));
				}

			template<class S=Callback, typename member_test<decltype(&S::key)>::type=0>
			void keyRegister(has_member)
				{
				glfwSetKeyCallback(m_handle,key);
				}

			ADVERTISE_MEMBER("key") void keyRegister(has_not_member){}
			


			static void codepoint(GLFWwindow* handle,unsigned int cp)
				{
				auto self=reinterpret_cast<Window*>(userPointerGet(handle));
				self->r_callback->codepoint(*self,static_cast<uint32_t>(cp));
				}

			template<class S=Callback, typename member_test<decltype(&S::codepoint)>::type=0>
			void codepointRegister(has_member)
				{
				glfwSetCharCallback(m_handle,codepoint);
				}

			ADVERTISE_MEMBER("codepoint") void codepointRegister(has_not_member){}
		};
	}

#endif
