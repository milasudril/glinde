//@	{
//@	 "targets":[{"name":"engine.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"engine.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_ENGINE_HPP
#define GLINDE_ENGINE_HPP

#include "display.hpp"
#include "scene.hpp"
#include "uicallback.hpp"

namespace Glinde
	{
	class Engine
		{
		public:
			Engine();
			void run();
			
		private:
			GLFWmm::Session m_session;
			UICallback m_cb; 
			Display m_mainwin;
			Scene m_scene;
		};
	}

#endif
