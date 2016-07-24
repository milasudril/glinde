//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"usereventhandler.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"usereventhandler.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_USEREVENTHANDLER_H
#define GLINDE_USEREVENTHANDLER_H

#include "window.h"

namespace Glinde
	{
	class World;

	class UserEventHandler:public Window::EventHandler
		{
		public:
			explicit UserEventHandler(World& world):
				r_world(&world),m_move_flags(0)
				{}

			void onKeyDown(Window& source,uint8_t scancode);
			void onKeyUp(Window& source,uint8_t scancode);
			void onTextInput(Window& source,uint32_t codepoint){}

			void onMouseMove(Window& source,double x,double y);
			void onMouseDown(Window& source,int button);
			void onMouseUp(Window& source,int button){}

			void onResize(Window& source,int width,int height){}

		private:
			World* r_world;

			unsigned int keyToFlag(uint8_t key);
			void objectUpdate();
			unsigned int m_move_flags;
			static constexpr unsigned int MOVE_FORWARD=1;
			static constexpr unsigned int MOVE_BACKWARD=2;
			static constexpr unsigned int STRAFE_LEFT=4;
			static constexpr unsigned int STRAFE_RIGHT=8;
			static constexpr unsigned int JUMP=16;
			static constexpr unsigned int MENU_MODE=32;
		};
	}

#endif
