//@ {
//@	 "targets":[{"name":"session.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"session.o","rel":"implementation"}]
//@	}

#ifndef GLFWMM_SESSION_HPP
#define GLFWMM_SESSION_HPP

#include "membercheck.hpp"
#include <GLFW/glfw3.h>
#include <type_traits>

namespace GLFWmm
	{
	class Session
		{
		public:
			Session(const Session&)=delete;
			Session& operator=(const Session&)=delete;

			Session(Session&& a) noexcept:m_dead(0)
				{
				a.m_dead=1;
				m_gl_ver_max_major=a.m_gl_ver_max_major;
				m_gl_ver_max_minor=a.m_gl_ver_max_minor;
				}

			Session& operator=(Session&& a) noexcept
				{
				m_dead=0;
				a.m_dead=1;
				m_gl_ver_max_major=a.m_gl_ver_max_major;
				m_gl_ver_max_minor=a.m_gl_ver_max_minor;
				return *this;
				}

			Session():Session(error_handler_default)
				{}

			explicit Session(GLFWerrorfun error_handler):m_dead(0)
				{
				glfwSetErrorCallback(error_handler);
				glfwInit();
				glfwSetErrorCallback(NULL);
				gl_version_maximize();
				glfwSetErrorCallback(error_handler);
				}

			~Session() noexcept
				{
				if(!m_dead)
					{glfwTerminate();}
				}

			int glVersionMaxMajor() const noexcept
				{return  m_gl_ver_max_major;}
			
			int glVersionMaxMinor() const noexcept
				{return  m_gl_ver_max_minor;}

			static const char* versionGet() noexcept
				{return glfwGetVersionString();}

			Session& swapInterval(int time) noexcept
				{
				glfwSwapInterval(time);
				return *this;
				}

			void eventsPoll()
				{glfwPollEvents();}

			void waitEvents()
				{glfwWaitEvents();}
 
			static void error_handler_default(int code,const char* message);

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

			static void creationHintsDefault(Session&) noexcept
				{return glfwDefaultWindowHints();}

			template<class CreationHints>
			void creationHints(const CreationHints& hints) noexcept
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

		private:
			void gl_version_maximize();

			int m_gl_ver_max_major;
			int m_gl_ver_max_minor;
			bool m_dead;


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
				glfwWindowHint(GLFW_SRGB_CAPABLE,hints.srgb);
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
				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,hints.clientAPIForwardCompat);
				};

			template<class CreationHints>
			ADVERTISE_MEMBER("clientAPIForwardCompat")
			static void clientAPIForwardCompatSet(const CreationHints& hints,has_not_member) noexcept{}
		};
	}

#endif