//@	{"targets":[{"name":"uicallback.hpp","type":"include"}]}

#ifndef GLINDE_UICALLBACK_HPP
#define GLINDE_UICALLBACK_HPP

#include "renderlist.hpp"
#include "inputhandler.hpp"
#include "glfwmm/window.hpp"

namespace Glinde
	{
	class UICallback
		{
		public:
			using Window=GLFWmm::Window<UICallback>;

			UICallback(RenderList& renderlist,InputHandler& input_handler):
				r_renderlist(&renderlist),r_input_handler(&input_handler)
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

		private:
			RenderList* r_renderlist;
			InputHandler* r_input_handler;
		};
	}

#endif
