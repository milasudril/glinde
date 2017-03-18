//@	{
//@	 "targets":[{"name":"engine.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"engine.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_ENGINE_HPP
#define GLINDE_ENGINE_HPP

#include "display.hpp"
#include "uicallback.hpp"
#include "console.hpp"
#include "image.hpp"
#include "messagequeue.hpp"
#include "handle.hpp"
#include "logwriterconsole.hpp"
#include "consoledisplay.hpp"
#include "consoleinputhandler.hpp"

namespace Glinde
	{
	class Timer;
	class Engine
		{
		public:
			Engine();
			~Engine()
				{glFinish();}

			void run(Timer&& timer)
				{run(timer);}

			void run(Timer& timer);
			
		private:
			MessageQueue m_queue;
			Handle<unsigned int,-1,decltype(&logQueueDetach)> m_queue_guard;
			Console m_console;
			ConsoleInputHandler m_con_input;
			LogWriterConsole m_con_writer;			
			Image m_charmap;

			Message m_msg;
			uint64_t m_frame_current;
			GLFWmm::Session m_session;
			UICallback m_cb; 
			Display m_mainwin;
			ConsoleDisplay m_con_display;
			RenderList m_renderlist;
		};
	}

#endif
