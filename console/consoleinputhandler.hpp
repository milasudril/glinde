//@	{
//@	 "targets":[{"name":"consoleinputhandler.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"consoleinputhandler.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_CONSOLEINPUTHANDLER_HPP
#define GLINDE_CONSOLEINPUTHANDLER_HPP

#include "../inputhandler.hpp"
#include "console.hpp"
#include "../arraydynamic.hpp"
#include "../status.hpp"

namespace Glinde
	{
	class Timeinfo;
	class ConsoleInputHandler:public InputHandler
		{
		public:
			ConsoleInputHandler(Console& con):r_con(&con),m_ready(0)
				{}

			void codepoint(uint32_t cp) noexcept
				{
				if(m_ready)
					{
					r_con->write(cp);
					m_input_buffer.append(cp);
					}
				}

			void key(int scancode,GLFWmm::WindowBase::Action action
				,unsigned int modifiers);

			void operator()(const Timeinfo& ti,Status status) noexcept
				{
				if(status==Status::READY)
					{
					m_ready=1;
					r_con->writeUTF8("»");
					}
				else
					{m_ready=0;}
				}


		private:
			Console* r_con;
			ArrayDynamic<uint32_t> m_input_buffer;
			bool m_ready;
		};
	}

#endif
