//@	{
//@	 "dependencies_extra":[{"ref":"logwriterconsole.o","rel":"implementation"}]
//@ ,"targets":[{"name":"logwriterconsole.hpp","type":"include"}]
//@	}

#ifndef GLINDE_LOGWRITERCONSOLE_HPP
#define GLINDE_LOGWRITERCONSOLE_HPP

#include "../log.hpp"

namespace Glinde
	{
	class Console;

	class LogWriterConsole:public Log::Writer
		{
		public:
			LogWriterConsole(Console& con):r_con(con){}
			void write(Log::MessageType type,const char* message) noexcept;

		private:
			Console& r_con;
		};
	}

#endif
