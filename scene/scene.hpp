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
				{m_buffer_out.framebufferResize(width,height);}

			void render(Display& disp,const Timeinfo& ti) const noexcept
				{}

		private:
			SceneRenderer m_renderer;
			mutable CameraBuffer m_buffer_out;
		};
	}

#endif
