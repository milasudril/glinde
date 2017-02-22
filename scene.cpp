//@	{
//@	"targets":[{"name":"scene.o","type":"object"}]
//@	}

#include "scene.hpp"
#include "display.hpp"

using namespace Glinde;

Scene::Scene()
	{
	glEnable(GL_BLEND);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.5,0.5,0.5,1.0);
	}

void Scene::fbResize(int width,int height)
	{
	m_hud.fbResize(width,height);
	glViewport(0,0,width,height);
	}

void Scene::render(Display& disp) const noexcept
	{
	glClear(GL_COLOR_BUFFER_BIT);


	m_hud.render(disp);
	}
