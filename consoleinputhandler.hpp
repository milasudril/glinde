//@	{
//@	 "targets":[{"name":"consoleinputhandler.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"consoleinputhandler.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_CONSOLEINPUTHANDLER_HPP
#define GLINDE_CONSOLEINPUTHANDLER_HPP

#include "inputhandler.hpp"
#include "console.hpp"
#include "arraydynamic.hpp"

namespace Glinde
	{
	class ConsoleInputHandler:public InputHandler
		{
		public:
			ConsoleInputHandler(Console& con):r_con(&con)
				{}

			void codepoint(uint32_t cp) noexcept
				{
				r_con->write(cp);
				m_input_buffer.append(cp);
				}

			void key(int scancode,GLFWmm::WindowBase::Action action
				,unsigned int modifiers);

		private:
			Console* r_con;
			ArrayDynamic<uint32_t> m_input_buffer;
		};
	}

#endif
