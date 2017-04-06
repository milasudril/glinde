//@	{
//@	 "targets":[{"name":"consoleinputhandler.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"consoleinputhandler.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_CONSOLEINPUTHANDLER_HPP
#define GLINDE_CONSOLEINPUTHANDLER_HPP

#include "../inputhandler.hpp"
#include "consolebuffer.hpp"
#include "../arraydynamic.hpp"
#include "../status.hpp"

namespace Glinde
	{
	class Timeinfo;
	class CommandProcessor;
	class ConsoleInputHandler:public InputHandler
		{
		public:
			explicit ConsoleInputHandler(ConsoleBuffer& con,CommandProcessor& cmdproc):
				r_con(&con),r_cmdproc(&cmdproc),m_status(Status::WAITING)
				{}

			void codepoint(uint32_t cp) noexcept
				{
				switch(m_status)
					{
					case Status::READY:
						r_con->write(cp);
						m_input_buffer.append(cp);
						break;
					default:
						break;
					}
				}

			void key(int scancode,GLFWmm::WindowBase::Action action
				,unsigned int modifiers);

			void operator()(const Timeinfo& ti,Status s,int) noexcept
				{
				status(s);
				}

			void status(Status s)
				{
				m_status=s;
				switch(s)
					{
					case Status::READY:
						m_input_buffer.clear();
						r_con->writeUTF8("»");
						break;
					default:
						break;
					}
				}

			ConsoleBuffer& consoleBuffer() noexcept
				{return *r_con;}

		private:
			ConsoleBuffer* r_con;
			CommandProcessor* r_cmdproc;
			ArrayDynamic<uint32_t> m_input_buffer;
			Status m_status;
		};
	}

#endif
