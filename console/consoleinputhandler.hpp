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
//@	 "targets":[{"name":"consoleinputhandler.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"consoleinputhandler.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_CONSOLEINPUTHANDLER_HPP
#define GLINDE_CONSOLEINPUTHANDLER_HPP

#include "../inputhandler.hpp"
#include "consolebuffer.hpp"
#include "../storage/arraydynamic.hpp"
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
