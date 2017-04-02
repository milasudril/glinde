//@	{
//@	 "dependencies_extra":[{"ref":"logwriterconsole.o","rel":"implementation"}]
//@ ,"targets":[{"name":"logwriterconsole.hpp","type":"include"}]
//@	}

#ifndef GLINDE_LOGWRITERCONSOLE_HPP
#define GLINDE_LOGWRITERCONSOLE_HPP

#include "../log/log.hpp"

namespace Glinde
	{
	class Console;

	class LogWriterConsole:public Log::Writer
		{
		public:
			explicit LogWriterConsole(Console& con):r_con(con),prog_state(0){}
			void write(Log::MessageType type,const char* message) noexcept;
			void progress(const char* message,double x) noexcept;

		private:
			Console& r_con;
			int prog_state;
		};
	}

#endif
