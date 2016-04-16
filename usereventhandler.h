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
			explicit UserEventHandler(WorldObject& player,double dt):
				r_player(&player),m_dt(dt),m_move_flags(0)
				{}

			void onKeyDown(Window& source,uint8_t scancode);
			void onKeyUp(Window& source,uint8_t scancode);
			void onTextInput(Window& source,uint32_t codepoint){}

			void onMouseMove(Window& source,double x,double y);
			void onMouseDown(Window& source,int button){}
			void onMouseUp(Window& source,int button){}

			void onResize(Window& source,int width,int height){}

		private:
			WorldObject* r_player;
			double m_dt;


			unsigned int keyToFlag(uint8_t key);
			unsigned int m_move_flags;
			static constexpr unsigned int MOVE_FORWARD=1;
			static constexpr unsigned int MOVE_BACKWARD=2;
			static constexpr unsigned int STRAFE_LEFT=4;
			static constexpr unsigned int STRAFE_RIGHT=8;
		};
	}

#endif
