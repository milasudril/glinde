//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"logwriterconsole.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"logwriterconsole.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_LOGWRITERCONSOLE_H
#define GLINDE_LOGWRITERCONSOLE_H

#include "log.h"

namespace Glinde
	{
	class Engine;

	class LogWriterConsole:public Log::Writer
		{
		public:
			LogWriterConsole(Engine& engine):r_engine(engine){}
			void write(Log::MessageType type,const char* message) noexcept;

		private:
			Engine& r_engine;
		};
	}

#endif
