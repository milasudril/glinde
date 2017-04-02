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
#include "console/console.hpp"
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

			void operator()(const Timeinfo& ti,GameLoader* loader);

			void operator()(const Timeinfo& ti,std::unique_ptr<Game>&& game);
			
		private:
			MessageQueue m_queue;
			Handle<unsigned int,-1,decltype(&logQueueDetach)> m_queue_guard;
			Console m_console;
			ConsoleInputHandler m_con_input;
			LogWriterConsole m_con_writer;

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
			std::unique_ptr<Game> m_game;
			std::unique_ptr<Thread<GameLoader>> m_game_loader;

			unsigned int m_con_index;
			volatile bool m_stop;
		};
	}

#endif
