//@	{
//@	 "targets":[{"name":"engine.o","type":"object"}]
//@	}

#include "engine.hpp"
#include "blob.hpp"
#include "vgacell.hpp"
#include "timer/timer.hpp"
#include "io/memoryreader.hpp"
#include "angle/init.hpp"
#include "log/logwriter.hpp"

#include <maike/targetinclude.hpp>
#include <utility>
#include <cstdio>

using namespace Glinde;

namespace
	{
	struct GLFWCreationHints
		{
		static constexpr bool srgb=1;
		};

	void glfwmm_error(int code,const char* message)
		{
		throw ErrorMessage("Failed to initialize the session. #0;.",{message});
		}

	GLFWmm::Session sessionCreate()
		{
		Glinde::logWrite(Glinde::Log::MessageType::INFORMATION,"Initializing GLFW version #0;",
			{GLFWmm::Session::versionGet()});
		GLFWmm::Session ret(glfwmm_error);
		Glinde::logWrite(Glinde::Log::MessageType::INFORMATION
			,"GLFW claims that the system supports OpenGL® versions ≤ #0;.#1;"
			,{ret.glVersionMaxMajor(),ret.glVersionMaxMinor()});
		ret.creationHints(GLFWCreationHints{});
		return std::move(ret);
		}
	}

GLINDE_BLOB(charmap,"charmap.png");
GLINDE_BLOB(consoletest,"consoletest.bin");

Engine::Engine():
	 m_queue_guard(logQueueAttach(m_queue),&logQueueDetach)
	,m_console(25,80)
	,m_con_input(m_console)
	,m_con_writer(m_console)
	,m_charmap(MemoryReader(charmap_begin,charmap_end),0)
	,m_frame_current(0)
	,m_session(sessionCreate())
	,m_cb(m_renderlist,m_con_input)
	,m_mainwin(m_cb,m_session)
	,m_con_display(m_charmap,m_console)
	{
	glfwSwapInterval(0);
	auto size_fb=m_mainwin.sizeFb();
	m_cb.framebufferSizeChanged(m_mainwin,size_fb.width,size_fb.height);
	auto con_id=m_renderlist.insertOnTop(m_con_display);
	m_renderlist.activate(con_id);
	logWriterAttach(m_con_writer);
/*	m_console.writeVGADump(Range<const VGACell>
		{reinterpret_cast<const VGACell*>(consoletest_begin),80*25});*/
	}

void Engine::run(Timer& timer)
	{
	m_mainwin.inputMode(GLFWmm::WindowBase::MouseButtonMode::STICKY)
		.inputMode(GLFWmm::WindowBase::KeyMode::STICKY);
	auto dt=timer.delay();
	auto tau=m_frame_current;
	auto msg_header=m_msg_header;
	do
		{
		auto now=tau*dt; // + t_0;
		if(msg_header.arrivalTime()<=now)
			{
			if(msg_header.valid())
				{m_queue.process(msg_header);}
			while(m_queue.get(msg_header))
				{
				if(msg_header.arrivalTime() > now)
					{break;}
				m_queue.process(msg_header);
				}
			}
		m_session.eventsPoll();
		m_renderlist.render(m_mainwin,tau);
		m_mainwin.buffersSwap();
		timer.wait();
		++tau;
		}
	while(!m_mainwin.shouldClose());
	m_msg_header=msg_header;
	m_frame_current=tau;
	}
