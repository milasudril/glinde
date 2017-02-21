//@	{
//@	"targets":[{"name":"scene.o","type":"object"}]
//@	}

#include "scene.hpp"
#include "display.hpp"

using PageComposer::Surface;

namespace PageComposer
	{
	inline constexpr auto gl_format(Surface::Pixel x)
		{return GL_RGBA;}

	inline constexpr auto gl_type(Surface::Pixel x)
		{return GL_UNSIGNED_INT_8_8_8_8_REV;}
	}

using namespace Glinde;

void Scene::render(Display& disp)
	{
	auto size=disp.sizeFbGet();

	if(size!=m_size_old)
		{
		m_hud_texture.realloc(size.width,size.height);
		m_hud_surface=PageComposer::Surface(size.width,size.height);
		m_hud_context=PageComposer::RenderContext(m_hud_surface);
		m_hud_texture.dataSet(m_hud_surface.data(),size.width,size.height);

		glViewport(0,0,size.width,size.height);
		m_size_old=size;
		}

	m_hud_pos_display.text("Hello, World");
	m_hud_layers.render();
//	m_hud_surface.save("test.png");
	m_hud_texture.dataSet(m_hud_surface.data(),size.width,size.height);
	disp.draw(m_hud_texture);
	}
