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
//@	"targets":[{"name":"overlay.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"overlay.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_OVERLAY_HPP
#define GLINDE_OVERLAY_HPP

#include "stringkey.hpp"
#include "renderobject.hpp"
#include "angle/texture2d.hpp"
#include "glfwmm/size.hpp"
#include "pagecomposer/paragraph.hpp"
#include "pagecomposer/textrenderer.hpp"
#include "pagecomposer/layerstack.hpp"

#include <memory>
#include <map>

namespace Glinde
	{
	class Overlay:public RenderObject
		{
		public:
			Overlay();

			void framebufferResize(int width,int height);

			void render(Display& disp,const Timeinfo& ti) const noexcept;

			PageComposer::Paragraph& paragraph(Stringkey key) noexcept
				{
				auto i=m_paragraphs.find(key);
				if(i!=m_paragraphs.end())
					{return i->second;}
				auto ip=m_paragraphs.insert(std::pair<Stringkey,PageComposer::Paragraph>{key,PageComposer::Paragraph(m_text)});
				m_layers.push(PageComposer::Layer(ip.first->second));
				return ip.first->second;
				}

		private:
			mutable Angle::Texture2D m_texture;
			PageComposer::Surface m_surface;
			std::unique_ptr<PageComposer::Surface::Pixel[]> m_texbuff;
			PageComposer::RenderContext m_context;
			PageComposer::TextRenderer m_text;

			std::map<Stringkey,PageComposer::Paragraph> m_paragraphs;
			PageComposer::LayerStack m_layers;			
		};
	}

#endif
