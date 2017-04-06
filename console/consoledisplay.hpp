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
//@	{
//@	 "targets":[{"name":"consoledisplay.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"consoledisplay.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_CONSOLEDISPLAY_HPP
#define GLINDE_CONSOLEDISPLAY_HPP

#include "../renderobject.hpp"
#include "../display.hpp"
#include "consolerenderer.hpp"

namespace Glinde
	{
	class ConsoleDisplay final:public RenderObject
		{
		public:
			explicit ConsoleDisplay(ConsoleBuffer&& con)=delete;
			explicit ConsoleDisplay(const ConsoleBuffer& con);

			void framebufferResize(int width,int height)
				{}

			void render(Display& disp,const Timeinfo& ti) const noexcept
				{
				m_con_renderer.render(m_con_texture,ti);
				Angle::Framebuffer::framebufferDefault()
					.bind(Angle::Framebuffer::Target::READDRAW);
				auto s=disp.sizeFb();
				glViewport(0,0,s.width,s.height);
				disp.draw(m_con_texture);
				}

		private:
			ConsoleRenderer m_con_renderer;
			mutable Angle::Texture2D m_con_texture;
		};
	}

#endif
