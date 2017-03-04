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

namespace Glinde
	{
	class Timer;
	class Engine
		{
		public:
			Engine();

			void run(Timer&& timer)
				{run(timer);}

			void run(Timer& timer);
			
		private:
			Console m_console;
			Image m_charmap;

			GLFWmm::Session m_session;
			UICallback m_cb; 
			Display m_mainwin;
			RenderList m_renderlist;
		};
	}

#endif
