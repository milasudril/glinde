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
//@	 "targets":[{"name":"cmdprocmain.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"cmdprocmain.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_CMDPROCMAIN_HPP
#define GLINDE_CMDPROCMAIN_HPP

#include "commandprocessor.hpp"
#include "enginebase.hpp"
#include "console/consoleinputhandler.hpp"

namespace Glinde
	{
	class Display;

	class CmdProcMain:public CommandProcessor
		{
		public:
			explicit CmdProcMain(EngineBase& eng,Display& disp):
				r_engine(&eng),r_display(&disp)
				{}

			void process(ConsoleInputHandler& coninput,const char* cmd);

		private:
			EngineBase* r_engine;
			Display* r_display;
		};
	}

#endif