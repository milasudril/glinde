//@	{
//@	"targets":[{"name":"renderlist.o","type":"object"}]
//@	}

#include "renderlist.hpp"
#include "display.hpp"

using namespace Glinde;

RenderList::RenderList()
	{
	glEnable(GL_BLEND);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.5,0.5,0.5,1.0);
	}

void RenderList::framebufferResize(int width,int height)
	{
	glViewport(0,0,width,height);
	}

void RenderList::render(Display& disp) const noexcept
	{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	}
