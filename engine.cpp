//@	{
//@	 "targets":[{"name":"engine.o","type":"object"}]
//@	}

#include "engine.hpp"
#include "blob.hpp"
#include "vgacell.hpp"
#include "status.hpp"
#include "time/timeinfo.hpp"
#include "time/timer.hpp"
#include "time/executiontimer.hpp"
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
	,m_con_input(m_console,m_cmdproc)
	,m_con_writer(m_console)
	,m_charmap(MemoryReader(charmap_begin,charmap_end),0)
	,m_t_0(0)
	,m_session(sessionCreate())
	,m_cb(m_renderlist,m_con_input,*this)
	,m_mainwin(m_cb,m_session)
	,m_cmdproc(*this,m_mainwin)
	,m_con_display(m_charmap,m_console)
	{
	glfwSwapInterval(0);
	auto size_fb=m_mainwin.sizeFb();
	m_cb.framebufferSizeChanged(m_mainwin,size_fb.width,size_fb.height);
	auto con_id=m_renderlist.insertOnTop(m_con_display);
	m_renderlist.activate(con_id);
	m_con_index=logWriterAttach(m_con_writer);
	m_queue.post(0,Message{m_con_input,Status::READY});
	}

Engine::~Engine()
	{logWriterDetach(m_con_index);}

void Engine::consoletest()
	{
	m_console.writeVGADump(Range<const VGACell>
		{reinterpret_cast<const VGACell*>(consoletest_begin),80*25});
	m_con_input.status(Status::READY_AFTER_KEY);
	}

void Engine::run(Timer& timer)
	{
	m_mainwin.inputMode(GLFWmm::WindowBase::MouseButtonMode::STICKY)
		.inputMode(GLFWmm::WindowBase::KeyMode::STICKY);
	auto dt=timer.delay();
	auto t=m_t_0;
	auto msg_header=m_msg_header;
	m_stop=0;
	size_t counter=0;
	ExecutionTimer et;
	do
		{
		Timeinfo now(t,dt);
		try
			{
			m_session.eventsPoll();
			if(msg_header.arrivalTime()<=t)
				{
				if(msg_header.valid())
					{m_queue.process(msg_header,now);}
				while(m_queue.get(msg_header))
					{
					if(msg_header.arrivalTime() > t)
						{break;}
					m_queue.process(msg_header,now);
					}
				}
			}
		catch(const ErrorMessage& err)
			{
			logWrite(Glinde::Log::MessageType::ERROR,"Error: #0;",{err.messageGet()});
			m_queue.post(0,Message{m_con_input,Status::READY});
			}
		
		if(counter%64==0)
			{
			fprintf(stderr,"%.15g\n",et.value()/(64.0*dt));
			et.reset();
			}

		m_renderlist.render(m_mainwin,now);
		m_mainwin.buffersSwap();
		timer.wait();
		++counter;
		t+=dt;
		}
	while(!m_stop);
	m_msg_header=msg_header;
	m_t_0=t;
	}
