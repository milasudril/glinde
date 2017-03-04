//@	{
//@	 "targets":[{"name":"engine.o","type":"object"}]
//@	}

#include "engine.hpp"
#include "blob.hpp"
#include "timer.hpp"
#include "memoryreader.hpp"
#include "angle/init.hpp"

#include <maike/targetinclude.hpp>
#include <utility>
#include <cstdio>

using namespace Glinde;

namespace
	{
	struct GLFWCreationHints
		{
		explicit constexpr GLFWCreationHints(const Angle::VersionRequest& req):
			 clientAPI(GLFWmm::Session::ClientAPI::OPENGL)
			,clientAPIVersionMajor(req.major)
			,clientAPIVersionMinor(req.minor)
			,clientAPIForwardCompat(req.forward_compatible)
			,clientAPIProfile(GLFWmm::Session::ClientAPIProfile::ANY)
			{
			switch(req.profile)
				{
				case Angle::VersionRequest::Profile::CORE:
					clientAPIProfile=GLFWmm::Session::ClientAPIProfile::CORE;
					break;
				case Angle::VersionRequest::Profile::COMPAT:
					clientAPIProfile=GLFWmm::Session::ClientAPIProfile::COMPAT;
					break;
				case Angle::VersionRequest::Profile::ANY:
				default:
					clientAPIProfile=GLFWmm::Session::ClientAPIProfile::ANY;
					break;
				}
			}

		GLFWmm::Session::ClientAPI clientAPI;
		int clientAPIVersionMajor;
		int clientAPIVersionMinor;
		bool clientAPIForwardCompat;
		GLFWmm::Session::ClientAPIProfile clientAPIProfile;
		static constexpr bool srgb=1;
		};

	void glfwmm_error(int code,const char* message)
		{
		throw ErrorMessage("Failed to initialize the session. #0;.",{message});
		}

	GLFWmm::Session sessionCreate()
		{
		GLFWmm::Session ret(glfwmm_error);
		ret.creationHints(GLFWCreationHints(Angle::gl_version_requirements()));
		return std::move(ret);
		}
	}

GLINDE_BLOB(charmap,"charmap.png");

Engine::Engine():m_console(25,80)
	,m_charmap(MemoryReader(charmap_begin,charmap_end),0)
	,m_session(sessionCreate())
	,m_cb(m_renderlist),m_mainwin(m_cb,m_session)
	{
	glfwSwapInterval(0);
	auto size_fb=m_mainwin.sizeFb();
	m_cb.framebufferSizeChanged(m_mainwin,size_fb.width,size_fb.height);
	}

void Engine::run(Timer& timer)
	{
	m_mainwin.inputMode(GLFWmm::WindowBase::MouseButtonMode::STICKY)
		.inputMode(GLFWmm::WindowBase::KeyMode::STICKY);
	while(!m_mainwin.shouldClose())
		{
		m_session.eventsPoll();
		m_renderlist.render(m_mainwin);
		m_mainwin.buffersSwap();
		timer.wait();
		}
	}
