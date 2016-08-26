//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"windowgamedefault.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_WINDOWGAMEDEFAULT_H
#define GLINDE_WINDOWGAMEDEFAULT_H

#include "windowgame.h"
#include "renderer.h"
#include "consolerenderer.h"
#include "rendereroverlay.h"
#include "enginedefault.h"

namespace Glinde
	{
	class WindowGameDefault:public WindowGame
		{
		public:
			WindowGameDefault(const char* title,int width,int height
				,EventHandler& handler):WindowGame(title,width,height,handler)
				,r_engine(nullptr),w(width),h(height)
				{
				cursorHide();
				cursorHideAndGrab();
				}

			void redraw() noexcept
				{
				auto site=r_engine!=nullptr?r_engine->siteGet():nullptr;
				auto player=site!=nullptr?site->objectGet(site->playerGet()):nullptr;
				
				if(player!=nullptr && site!=nullptr)
					{renderer.sceneRender(*site,*player);}
				else
					{
					con_renderer.render();
					overlay.viewportSizeSet(w,h);
					overlay.render(con_renderer.textureGet());
					}
				}

			WindowGameDefault& engineSet(EngineDefault* e) noexcept
				{
				r_engine=e;
				con_renderer.consoleSet(&r_engine->consoleGet());
				return *this;
				}

			void resized(int width,int height)  noexcept
				{
				renderer.viewportSizeSet(width,height);
				w=width;
				h=height;
				}

		private:
			ConsoleRenderer con_renderer;
			RendererOverlay overlay;
			Renderer renderer;
			Console* r_con;
			EngineDefault* r_engine;
			const WorldObject* r_viewer;
			int w;
			int h;
		};
	}

#endif
