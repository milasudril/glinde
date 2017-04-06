//@	{"targets":[{"name":"inputhandler.hpp","type":"include"}]}

#ifndef GLINDE_INPUTHANDLER_HPP
#define GLINDE_INPUTHANDLER_HPP

#include "glfwmm/windowbase.hpp"

namespace Glinde
	{
	class InputHandler
		{
		public:
			virtual void codepoint(uint32_t cp){};
			virtual void key(int scancode,GLFWmm::WindowBase::Action action
				,unsigned int modifiers){};
		};
	}

#endif
