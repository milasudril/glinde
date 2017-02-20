//@	{
//@	 "targets":[{"name":"engine.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"engine.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_ENGINE_HPP
#define GLINDE_ENGINE_HPP

#define GLFWMM_NO_MEMBER_ADVERTISMENT
#include "glfwmm/window.hpp"

namespace Glinde
	{
	class Engine
		{
		public:
			Engine();
			void run();
			
		private:
			class UICallback
				{
				};

			GLFWmm::Session m_session;
			GLFWmm::Window<UICallback> m_mainwin;
			UICallback m_cb;
		};
	}

#endif
