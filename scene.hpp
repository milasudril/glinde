//@	{
//@	"targets":[{"name":"scene.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"scene.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_SCENE_HPP
#define GLINDE_SCENE_HPP

#include "hud.hpp"

namespace Glinde
	{
	class Display;

	class Scene
		{
		public:
			Scene();
			void fbResize(int width,int height);
			void render(Display& disp) const noexcept;

			Hud& hud() noexcept
				{return m_hud;}

		private:
			Hud m_hud;
		};
	}

#endif
