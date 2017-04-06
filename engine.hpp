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
//@	 "targets":[{"name":"engine.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"engine.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_ENGINE_HPP
#define GLINDE_ENGINE_HPP

#include "display.hpp"
#include "uicallback.hpp"
#include "handle.hpp"
#include "cmdprocmain.hpp"
#include "overlay.hpp"

#include "messagequeue/messagequeue.hpp"
#include "console/logwriterconsole.hpp"
#include "console/consolebuffer.hpp"
#include "console/consoledisplay.hpp"
#include "console/consoleinputhandler.hpp"
#include "scene/scene.hpp"
#include "thread/thread.hpp"

namespace Glinde
	{
	class Timer;

	class Engine:public EngineBase
		{
		public:
			Engine();
			~Engine();

			void run(Timer&& timer)
				{run(timer);}

			void run(Timer& timer);

			void stop()
				{m_stop=1;}

			void consoletest();

			void gameLoad(const char* archive);

			void operator()(const Timeinfo& ti,GameLoader* loader,int x);

			void operator()(const Timeinfo& ti,std::unique_ptr<Game>&& game,int x);
			
		private:
			MessageQueue m_queue;
			Handle<unsigned int,-1,decltype(&logQueueDetach)> m_queue_guard;
			ConsoleBuffer m_console;
			ConsoleInputHandler m_con_input;
			LogWriterConsole m_con_writer;
			unsigned int m_con_writer_index;


			MessageHeader m_msg_header;
			double m_t_0;
			GLFWmm::Session m_session;
			UICallback m_cb; 
			Display m_mainwin;

			CmdProcMain m_cmdproc;

			ConsoleDisplay m_con_display;
			Overlay m_overlay;
			Scene m_scene;
			RenderList m_renderlist;

			unsigned int m_condisp_id;
			unsigned int m_overlay_id;
			unsigned int m_scene_id;

			std::unique_ptr<Game> m_game;
			std::unique_ptr<Thread<GameLoader>> m_game_loader;
			volatile bool m_stop;
		};
	}

#endif
