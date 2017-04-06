/*
Glinde
Copyright (C) 2017  Torbjörn Rathsman

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/
//@	{"targets":[{"name":"uicallback.hpp","type":"include"}]}

#ifndef GLINDE_UICALLBACK_HPP
#define GLINDE_UICALLBACK_HPP

#include "renderlist.hpp"
#include "inputhandler.hpp"
#include "enginebase.hpp"
#include "glfwmm/window.hpp"

#include <cstdio>

namespace Glinde
	{
	class UICallback
		{
		public:
			using Window=GLFWmm::Window<UICallback>;

			explicit UICallback(RenderList& renderlist,InputHandler& input_handler
				,EngineBase& engine):
				r_renderlist(&renderlist),r_input_handler(&input_handler),r_engine(&engine)
				{}

			void framebufferSizeChanged(Window& win,int x,int y)
				{
				r_renderlist->framebufferResize(x,y);
				}

			void cursorPos(Window& win,double x,double y)
				{

				}

			void mouseButton(Window& win,int button,Window::Action action
				,unsigned int modifiers)
				{
				if(action==Window::Action::PRESS)
					{

					}
				}

			void codepoint(Window& win,uint32_t cp)
				{r_input_handler->codepoint(cp);}

			void key(Window& win,int scancode,GLFWmm::WindowBase::Action action
					,unsigned int modifiers)
				{
				if(scancode==9 && modifiers==0)
					{fprintf(stderr,"Return to previous state...\n");}
				else
					{r_input_handler->key(scancode,action,modifiers);}
				}

			void closing(Window& win)
				{
				win.shouldClose(0);
				r_engine->stop();
				}

		private:
			RenderList* r_renderlist;
			InputHandler* r_input_handler;
			EngineBase* r_engine;
		};
	}

#endif
