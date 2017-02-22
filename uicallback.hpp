//@	{"targets":[{"name":"uicallback.hpp","type":"include"}]}

#ifndef GLINDE_UICALLBACK_HPP
#define GLINDE_UICALLBACK_HPP

#include "glfwmm/window.hpp"
#include "scene.hpp"

namespace Glinde
	{
	class UICallback
		{
		public:
			UICallback(Scene& scene):r_scene(&scene)
				{}

			void framebufferSizeChanged(GLFWmm::Window<UICallback>& win,int x,int y)
				{
				r_scene->fbResize(x,y);
				}

		private:
			Scene* r_scene;
		};
	}

#endif
