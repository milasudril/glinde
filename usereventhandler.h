#ifdef __WAND__
target[name[usereventhandler.h] type[include]]
dependency[usereventhandler.o]
#endif

#ifndef GLINDA_USEREVENTHANDLER_H
#define GLINDA_USEREVENTHANDLER_H

#include "window.h"

namespace Glinda
	{
	class WorldObject;

	class UserEventHandler:public Window::EventHandler
		{
		public:
			explicit UserEventHandler(WorldObject& player):r_player(&player){}

			void onKeyDown(Window& source,uint8_t scancode){}
			void onKeyUp(Window& source,uint8_t scancode){}
			void onTextInput(Window& source,uint32_t codepoint){}

			void onMouseMove(Window& source,double x,double y);
			void onMouseDown(Window& source,int button){}
			void onMouseUp(Window& source,int button){}

			void onResize(Window& source,int width,int height){}

		private:
			WorldObject* r_player;
		};
	}

#endif
