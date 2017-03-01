//@	{"targets":[{"name":"uicallback.hpp","type":"include"}]}

#ifndef GLINDE_UICALLBACK_HPP
#define GLINDE_UICALLBACK_HPP

#include "scene.hpp"
#include "glfwmm/window.hpp"

namespace Glinde
	{
	class UICallback
		{
		public:
			using Window=GLFWmm::Window<UICallback>;

			UICallback(Scene& scene):r_scene(&scene)
				{}

			void framebufferSizeChanged(Window& win,int x,int y)
				{
				r_scene->fbResize(x,y);
				}

			void cursorPos(Window& win,double x,double y)
				{
				auto pos=win.cursorPosition();
				auto& p=r_scene->hud().paragraph(Stringkey("position"));
				char buffer[20];
				sprintf(buffer,"(%.4g, %.4g)",pos.first,pos.second);
				p.text(buffer)
					.anchor(PageComposer::Vec2(0,0))
					.positionRelative(PageComposer::Vec2(0,0));
				}

			void mouseButton(Window& win,int button,Window::Action action
				,unsigned int modifiers)
				{
				if(action==Window::Action::PRESS)
					{

					}
				}

		private:
			Scene* r_scene;
		};
	}

#endif
