//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"engine.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_ENGINE_H
#define GLINDE_ENGINE_H

#include <cstdint>

namespace Glinde
	{
	class Engine
		{
		public:

			virtual void stop()=0;
			virtual void consoleWrite(const char* message)=0;
			virtual void consoleWrite(char ch)=0;
			virtual void consoleColorMaskSet(uint8_t colormask)=0;
		};
	}

#endif
