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
//@	 "dependencies_extra":[{"ref":"logwriterconsole.o","rel":"implementation"}]
//@ ,"targets":[{"name":"logwriterconsole.hpp","type":"include"}]
//@	}

#ifndef GLINDE_LOGWRITERCONSOLE_HPP
#define GLINDE_LOGWRITERCONSOLE_HPP

#include "../log/log.hpp"

namespace Glinde
	{
	class ConsoleBuffer;

	class LogWriterConsole:public Log::Writer
		{
		public:
			explicit LogWriterConsole(ConsoleBuffer& con):r_con(&con),prog_state(0){}
			void write(Log::MessageType type,const char* message) noexcept;
			void progress(const char* message,double x) noexcept;
			void progressInit() noexcept;
			void progressEnd() noexcept;

		private:
			ConsoleBuffer* r_con;
			int prog_state;
		};
	}

#endif
