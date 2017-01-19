//@	{"targets":[{"name":"window.hpp","type":"include"}]}

#ifndef GLFWMM_WINDOW_HPP
#define GLFWMM_WINDOW_HPP

#include <GLFW/glfw3.h>
#include <type_traits>


#ifndef GLFWMM_NO_MEMBER_ADVERTISMENT
#define ADVERTISE_MEMBER(membername) [[deprecated("Hint: add " #membername " to current class.")]]
#else
#define ADVERTISE_MEMBER(membername) 
#endif

namespace GLFWmm
	{
	struct has_not_member{};
	struct has_member:public has_not_member{};
	template<class X> struct member_test
		{typedef int type;};

	class WindowBase
		{
		public:
			enum class ClientAPI:int
				{
				 OPENGL=GLFW_OPENGL_API
			#ifdef GLFW_OPEGL_ES_API
				,OPENGL_ES=GLFW_OPEGL_ES_API
			#endif
			#ifdef GLFW_NO_API
				,NO_API=GLFW_NO_API
			#endif
				};

			enum class ClientAPIProfile:int
				{
				 ANY=GLFW_OPENGL_ANY_PROFILE
				,COMPAT=GLFW_OPENGL_COMPAT_PROFILE
				,CORE=GLFW_OPENGL_CORE_PROFILE
				};

			static void creationHintsDefault() noexcept
				{return glfwDefaultWindowHints();}

			template<class CreationHints>
			static void creationHints(const CreationHints& hints) noexcept
				{
				resizableSet(hints,has_member{});
				visibleSet(hints,has_member{});
				decoratedSet(hints,has_member{});
				srgbSet(hints,has_member{});
				clientAPISet(hints,has_member{});
				clientAPIVersionMajorSet(hints,has_member{});
				clientAPIVersionMinorSet(hints,has_member{});
				clientAPIForwardCompatSet(hints,has_member{});
				clientAPIProfileSet(hints,has_member{});
				}

			bool shouldClose() const noexcept
				{return glfwWindowShouldClose(m_handle);}

		protected:
			WindowBase(float width,float height,const char* title)
				{
				auto video_mode=glfwGetVideoMode(glfwGetPrimaryMonitor());
				m_handle=glfwCreateWindow(width*video_mode->width
					,height*video_mode->height,title,NULL,NULL);
				glfwSetWindowUserPointer(m_handle,this);
				}

			static void* userPointerGet(GLFWwindow* handle) noexcept
				{return glfwGetWindowUserPointer(handle);}
			GLFWwindow* m_handle;

		private:
			template<class CreationHints,typename member_test<decltype(CreationHints::resizable)>::type=0>
			static void resizableSet(const CreationHints& hints,has_member) noexcept
				{
				static_assert(std::is_same<std::decay_t<decltype(CreationHints::resizable)>,bool>::value,"");
				glfwWindowHint(GLFW_RESIZABLE,hints.resizable);
				}

			template<class CreationHints>
			ADVERTISE_MEMBER("bool resizable")
			static void resizableSet(const CreationHints& hints,has_not_member) noexcept{}



			template<class CreationHints,typename member_test<decltype(CreationHints::visible)>::type=0>
			static void visibleSet(const CreationHints& hints,has_member) noexcept
				{
				static_assert(std::is_same<std::decay_t<decltype(CreationHints::visible)>,bool>::value,"");
				glfwWindowHint(GLFW_VISIBLE,hints.visible);
				}

			template<class CreationHints>
			ADVERTISE_MEMBER("bool visible")
			static void visibleSet(const CreationHints& hints,has_not_member) noexcept{}



			template<class CreationHints,typename member_test<decltype(CreationHints::decorated)>::type=0>
			static void decoratedSet(const CreationHints& hints,has_member) noexcept
				{
				static_assert(std::is_same<std::decay_t<decltype(CreationHints::decorated)>,bool>::value,"");
				glfwWindowHint(GLFW_DECORATED,hints.decorated);
				}

			template<class CreationHints>
			ADVERTISE_MEMBER("bool decorated")
			static void decoratedSet(const CreationHints& hints,has_not_member) noexcept{}



			template<class CreationHints,typename member_test<decltype(CreationHints::srgb)>::type=0>
			static void srgbSet(const CreationHints& hints,has_member) noexcept
				{
				static_assert(std::is_same<std::decay_t<decltype(CreationHints::srgb)>,bool>::value,"");
				glfwWindowHint(GLFW_DECORATED,hints.srgb);
				}

			template<class CreationHints>
			ADVERTISE_MEMBER("bool srgb")
			static void srgbSet(const CreationHints& hints,has_not_member) noexcept{}



			template<class CreationHints,typename member_test<decltype(CreationHints::clientAPI)>::type=0>
			static void clientAPISet(const CreationHints& hints,has_member) noexcept
				{
				static_assert(std::is_same<std::decay_t<decltype(CreationHints::clientAPI)>,ClientAPI>::value,"");
				glfwWindowHint(GLFW_CLIENT_API,static_cast<std::underlying_type_t <ClientAPI>>(hints.clientAPI));
				}

			template<class CreationHints>
			ADVERTISE_MEMBER("ClientAPI clientAPI")
			static void clientAPISet(const CreationHints& hints,has_not_member) noexcept{}



			template<class CreationHints,typename member_test<decltype(CreationHints::clientAPIProfile)>::type=0>
			static void clientAPIProfileSet(const CreationHints& hints,has_member) noexcept
				{
				static_assert(std::is_same<std::decay_t<decltype(CreationHints::clientAPIProfile)>,ClientAPIProfile>::value,"");
				glfwWindowHint(GLFW_OPENGL_PROFILE,static_cast<std::underlying_type_t <ClientAPIProfile>>(hints.clientAPIProfile));
				}

			template<class CreationHints>
			ADVERTISE_MEMBER("ClientAPIProfile clientAPIProfile")
			static void clientAPIProfileSet(const CreationHints& hints,has_not_member) noexcept{}



			template<class CreationHints,typename member_test<decltype(CreationHints::clientAPIVersionMajor)>::type=0>
			static void clientAPIVersionMajorSet(const CreationHints& hints,has_member) noexcept
				{
				static_assert(std::is_same<std::decay_t<decltype(CreationHints::clientAPIVersionMajor)>,int>::value,"");
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,hints.clientAPIVersionMajor);
				};

			template<class CreationHints>
			ADVERTISE_MEMBER("clientAPIVersionMajor")
			static void clientAPIVersionMajorSet(const CreationHints& hints,has_not_member) noexcept{}



			template<class CreationHints,typename member_test<decltype(CreationHints::clientAPIVersionMinor)>::type=0>
			static void clientAPIVersionMinorSet(const CreationHints& hints,has_member)
				{
				static_assert(std::is_same<std::decay_t<decltype(CreationHints::clientAPIVersionMinor)>,int>::value,"");
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,hints.clientAPIVersionMinor);
				};

			template<class CreationHints>
			ADVERTISE_MEMBER("clientAPIVersionMinor")
			static void clientAPIVersionMinorSet(const CreationHints& hints,has_not_member) noexcept{}



			template<class CreationHints,typename member_test<decltype(CreationHints::clientAPIForwardCompat)>::type=0>
			static void clientAPIForwardCompatSet(const CreationHints& hints,has_member) noexcept
				{
				static_assert(std::is_same<std::decay_t<decltype(CreationHints::clientAPIForwardCompat)>,bool>::value,"");
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,hints.clientAPIForwardCompat);
				};

			template<class CreationHints>
			ADVERTISE_MEMBER("clientAPIForwardCompat")
			static void clientAPIForwardCompatSet(const CreationHints& hints,has_not_member) noexcept{}
		};

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
