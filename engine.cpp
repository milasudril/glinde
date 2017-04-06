/*
Glinde
Copyright (C) 2017  Torbjörn Rathsman

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/
//@	{
//@	 "targets":[{"name":"engine.o","type":"object"}]
//@	}

#include "engine.hpp"
#include "vgacell.hpp"
#include "status.hpp"
#include "storage/blob.hpp"
#include "time/timeinfo.hpp"
#include "time/timer.hpp"
#include "time/executiontimer.hpp"
#include "angle/init.hpp"
#include "log/logwriter.hpp"
#include "gamedata/gameloader.hpp"
#include <maike/targetinclude.hpp>
#include MAIKE_TARGET(projectinfo.hpp)
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

GLINDE_BLOB(consoletest,"consoletest.bin");

static void bannerDraw(ConsoleBuffer& con)
	{
	auto nc=con.colsCount();
	con.colorMask(0xf7)
		.fill(nc,U' ');
	String text;
	text.append(ProjectInfo::name())
		.append(' ').append(ProjectInfo::version());

	auto n=text.length();
	auto n_left=static_cast<int>( 0.5*(nc-8 - n) );
	con.colorMask(0xfd)
		.write(' ')
		.fill(n_left,U'〜')
		.write(' ')
		.colorMask(0xf9)
		.write(U'✦')
		.colorMask(0xfd)
		.write(' ')
		.colorMask(0xf5)
		.writeUTF8(text.begin())
		.colorMask(0xfd)
		.write(' ')
		.colorMask(0xf9)
		.write(U'✦')
		.colorMask(0xfd)
		.write(' ')
		.fill(nc-8-n-n_left,U'〜')
		.write(' ').colorMask(0xf0)
		.fill(nc,U'▄');
	}

Engine::Engine():
//	Attach the message queue to the global log so any log message is passed
//	to the queue.
	 m_queue_guard(logQueueAttach(m_queue),&logQueueDetach)

//	Console stuff
	,m_console(25,80)
	,m_con_input(m_console,m_cmdproc)
	,m_con_writer(m_console)

//	Initialize GLFW
	,m_session(sessionCreate())

//	Create UI callback and the main window
	,m_cb(m_renderlist,m_con_input,*this)
	,m_mainwin(m_cb,m_session)

//	Connect *this and m_mainwin to the main CommandProcessor
	,m_cmdproc(*this,m_mainwin)
	,m_con_display(m_console)
	{
	bannerDraw(m_console);

//	Connect console to logWriter
	m_con_writer_index=logWriterAttach(m_con_writer);

//	Reset the world clock
	m_t_0=0;
//	And stop check variable
	m_stop=0;
//	Currently, the render speed is controlled by an external timer
	glfwSwapInterval(0);

	auto size_fb=m_mainwin.sizeFb();

//	Broadcast framebuffer resize to affected parts
	m_cb.framebufferSizeChanged(m_mainwin,size_fb.width,size_fb.height);

//	Add all RenderObjects
	m_condisp_id=m_renderlist.insert(m_con_display,2);
	m_overlay_id=m_renderlist.insert(m_overlay,1);
	m_scene_id=m_renderlist.insert(m_scene,0);

//	Make the console visible
	m_renderlist.activate(m_condisp_id);

//	Notify the ConsoleInputHandler that it can process input.
	m_queue.post(0,Message{m_con_input,Status::READY,0});
	}

Engine::~Engine()
	{logWriterDetach(m_con_writer_index);}

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
		//TODO: Check exception policy for message processors. Should
		//	we really stop processing the remaining messages if one
		//	failed?

			m_session.eventsPoll();
			if(msg_header.arrivalTime()<=t)
				{
				if(msg_header.valid())
					{
					m_queue.process(msg_header,now);
					assert(!msg_header.valid());
					}
				while(m_queue.get(msg_header))
					{
					if(msg_header.arrivalTime() > t)
						{break;}
					assert(msg_header.valid());
					m_queue.process(msg_header,now);
					assert(!msg_header.valid());
					}
				}
			}
		catch(const ErrorMessage& err)
			{
			logWrite(Glinde::Log::MessageType::ERROR,"#0;",{err.messageGet()});
			m_queue.post(0,Message{m_con_input,Status::READY,0});
			}
		m_queue.swapBuffers();

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

void Engine::gameLoad(const char* archive)
	{
	m_game_loader=std::make_unique<Thread<GameLoader>>(GameLoader{archive,*this,m_queue});
	}

void Engine::operator()(const Timeinfo& ti,GameLoader* loader,int)
	{
	auto x=loader->error();
	m_game_loader.reset();
	throw x;
	}

void Engine::operator()(const Timeinfo& ti,std::unique_ptr<Game>&& game,int)
	{
	m_game=std::move(game);
	m_game_loader.reset();
	m_con_input.status(Status::READY);
	}
