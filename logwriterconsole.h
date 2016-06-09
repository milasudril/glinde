#ifdef __WAND__
target[name[logwriterconsole.h] type[include]]
dependency[logwriterconsole.o]
#endif

#ifndef GLINDE_LOGWRITERCONSOLE_H
#define GLINDE_LOGWRITERCONSOLE_H

#include "logwriter.h"

namespace Glinde
	{
	class Engine;

	class LogWriterConsole:public LogWriter
		{
		public:
			LogWriterConsole(Engine& engine):r_engine(engine){}
			void write(LogMessageType type,const char* message);

		private:
			Engine& r_engine;
		};
	}

#endif
