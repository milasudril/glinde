#ifdef __WAND__
target[name[windowgame.h] type[include]]
#endif

#ifndef GLINDA_WINDOWGAME_H
#define GLINDA_WINDOWGAME_H

#include "window.h"
#include "renderer.h"

namespace Glinda
	{
	class WindowGame:public Window
		{
		public:
			WindowGame(const char* title,int width,int height
				,EventHandler& handler
				,World& world
				,const Camera& eyes):Window(title,width,height,handler)
				,r_world(&world),r_eyes(&eyes)
				{}

			void redraw() noexcept
				{
				renderer.sceneRender(*r_world,*r_eyes);
				}

			void resized(int width,int height)  noexcept
				{
				renderer.viewportSizeSet(width,height);
				}

		private:
			Renderer renderer;
			World* r_world;
			const Camera* r_eyes;
		};
	}

#endif
