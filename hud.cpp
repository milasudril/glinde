//@	{
//@	"targets":[{"name":"hud.o","type":"object"}]
//@	}

#include "hud.hpp"
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

Hud::Hud():m_texture(1,Angle::TextureFormat::SRGB8_ALPHA8,1,1)
	,m_surface(1,1),m_texbuff(new PageComposer::Surface::Pixel[1])
	,m_context(m_surface)
	,m_text(m_context)
	,m_layers(m_context)
	{
/*	m_bodytext.color(PageComposer::Color(1,1,1,1));
	m_pos_display.style(m_bodytext);
	m_para.color(PageComposer::Color(0.25,0.25,0.25,0.75));
	auto pos_style=m_para;
	pos_style.alignment(PageComposer::ParaStyle::Alignment::RIGHT);
	m_pos_display.style(pos_style).positionRelative(PageComposer::Vec2(1,-1))
		.anchor(PageComposer::Vec2(1,-1));
	m_layers.push(PageComposer::Layer(m_pos_display));


	m_pos_display.text("Hello, World\nA longer piece of text");*/
	}

void Hud::fbResize(int width,int height)
	{
	m_texture.realloc(width,height);
	m_surface=PageComposer::Surface(width,height);
	m_texbuff.reset(new PageComposer::Surface::Pixel[width*height]);
	m_context=PageComposer::RenderContext(m_surface);
	m_texture.dataSet(m_surface.data(),width,height);
	m_layers.forcedRender();
	}

void Hud::render(Display& disp) const
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
