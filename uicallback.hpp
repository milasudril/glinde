//@	{"targets":[{"name":"uicallback.hpp","type":"include"}]}

#ifndef GLINDE_UICALLBACK_HPP
#define GLINDE_UICALLBACK_HPP

#include "renderlist.hpp"
#include "glfwmm/window.hpp"

namespace Glinde
	{
	class UICallback
		{
		public:
			using Window=GLFWmm::Window<UICallback>;

			UICallback(RenderList& renderlist):r_renderlist(&renderlist)
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

		private:
			RenderList* r_renderlist;
		};
	}

#endif
