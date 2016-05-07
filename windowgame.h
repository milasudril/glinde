#ifdef __WAND__
target[name[windowgame.h] type[include]]
#endif

#ifndef GLINDE_WINDOWGAME_H
#define GLINDE_WINDOWGAME_H

#include "window.h"
#include "renderer.h"
#include "world.h"

namespace Glinde
	{
	class WindowGame:public Window
		{
		public:
			WindowGame(const char* title,int width,int height
				,EventHandler& handler
				,World& world):Window(title,width,height,handler)
				,r_world(&world)
				{
			//	cursorHide();
				cursorHideAndGrab();
				}

			void redraw() noexcept
				{
				renderer.sceneRender(*r_world,r_world->playerGet());
				}

			void resized(int width,int height)  noexcept
				{
				renderer.viewportSizeSet(width,height);
				}

		private:
			Renderer renderer;
			World* r_world;
			const WorldObject* r_viewer;
		};
	}

#endif
