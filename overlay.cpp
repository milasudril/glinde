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
//@	"targets":[{"name":"overlay.o","type":"object"}]
//@	}

#include "overlay.hpp"
#include "display.hpp"

using PageComposer::Surface;

namespace PageComposer
	{
	inline constexpr auto gl_format(Surface::Pixel x)
		{return GL_BGRA;}

	inline constexpr auto gl_type(Surface::Pixel x)
		{return GL_UNSIGNED_INT_8_8_8_8_REV;}
	}

using namespace Glinde;

Overlay::Overlay():m_texture(Angle::TextureFormat::SRGB8_ALPHA8,1,1)
	,m_surface(1,1),m_texbuff(new PageComposer::Surface::Pixel[1])
	,m_context(m_surface)
	,m_text(m_context)
	,m_layers(m_context)
	{
	}

void Overlay::framebufferResize(int width,int height)
	{
	m_texture.realloc(width,height);
	m_surface=PageComposer::Surface(width,height);
	m_texbuff.reset(new PageComposer::Surface::Pixel[width*height]);
	m_context=PageComposer::RenderContext(m_surface);
	m_texture.dataSet(m_surface.data(),width,height);
	m_layers.forcedRender();
	}

void Overlay::render(Display& disp,const Timeinfo& ti) const noexcept
	{
	m_layers.render();
	auto rect=m_surface.dirtyRectangle();
	auto size=rect.size();
	if(size.x()>0)
		{
		auto si=rect.sizeInt();
		m_surface.rectangleGet(m_surface.dirtyRectangle(),m_texbuff.get());
		m_texture.dataSet(m_texbuff.get()
			,static_cast<int>(rect.min().x())
			,static_cast<int>(rect.min().y())
			,static_cast<int>(si.x())
			,static_cast<int>(si.y()));
		}
	disp.draw(m_texture);
	}
