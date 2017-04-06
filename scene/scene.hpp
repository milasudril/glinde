//@	{"targets":[{"name":"scene.hpp","type":"include"}]}

#ifndef GLINDE_SCENE_HPP
#define GLINDE_SCENE_HPP

#include "../renderobject.hpp"
#include "scenerenderer.hpp"

namespace Glinde
	{
	class Scene:public RenderObject
		{
		public:
			void framebufferResize(int width,int height)
				{m_renderer.framebufferResize(width,height);}

			void render(Display& disp,const Timeinfo& ti) const noexcept
				{}

		private:
			SceneRenderer m_renderer;

		};
	}

#endif
