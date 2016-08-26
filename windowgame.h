//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"windowgame.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_WINDOWGAME_H
#define GLINDE_WINDOWGAME_H

#include "window.h"

namespace Glinde
	{
	class EngineDefault;
	class WindowGame:public Window
		{
		public:
			WindowGame(const char* title,int width,int height
				,EventHandler& handler):Window(title,width,height,handler)
				{}

			virtual WindowGame& engineSet(EngineDefault* e) noexcept=0;
		};
	}

#endif
