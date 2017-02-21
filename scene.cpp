//@	{
//@	"targets":[{"name":"scene.o","type":"object"}]
//@	}

#include "scene.hpp"
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

void Scene::render(Display& disp)
	{
	glEnable(GL_BLEND);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.5,0.5,0.5,1.0);

		{
		auto size=disp.sizeFbGet();
		if(size!=m_size_old)
			{
			m_hud_texture.realloc(size.width,size.height);
			m_hud_surface=PageComposer::Surface(size.width,size.height);
			m_hud_texbuff.reset(new PageComposer::Surface::Pixel[size.width*size.height]);
			m_hud_context=PageComposer::RenderContext(m_hud_surface);
			m_hud_texture.dataSet(m_hud_surface.data(),size.width,size.height);

			glViewport(0,0,size.width,size.height);
			m_size_old=size;
			}
		}

	glClear(GL_COLOR_BUFFER_BIT);

		{
		m_hud_pos_display.text("Hello, World\nA longer piece of text");
		m_hud_layers.render();
		auto rect=m_hud_surface.dirtyRectangle();
		auto size=rect.size();
		if(size.x()>0)
			{
			auto si=rect.sizeInt();
			m_hud_surface.rectangleGet(m_hud_surface.dirtyRectangle(),m_hud_texbuff.get());
			m_hud_texture.dataSet(m_hud_texbuff.get(),rect.min().x(),rect.min().y(),si.x(),si.y());
			}
		}


	disp.draw(m_hud_texture);
	}
