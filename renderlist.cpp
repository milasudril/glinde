//@	{
//@	"targets":[{"name":"renderlist.o","type":"object"}]
//@	}

#include "renderlist.hpp"
#include "display.hpp"
#include "renderobject.hpp"
#include <algorithm>

using namespace Glinde;

RenderList::RenderList()
	{
	glEnable(GL_BLEND);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.25,0.25,0.25,1.0);
	}

void RenderList::framebufferResize(int width,int height)
	{
	glViewport(0,0,width,height);
	std::for_each(r_objects.begin(),r_objects.end(),[width,height](auto& x)
		{x.r_obj->framebufferResize(width,height);});
	}

void RenderList::render(Display& disp) const noexcept
	{
	if(m_dirty)
		{
		std::sort(r_objects.begin(),r_objects.end(),[](const auto& a,const auto& b)
			{return a.index<b.index;});
	//	Re-assign object indices
		std::for_each(r_objects.begin(),r_objects.end(),[this](const auto& obj)
			{m_id_to_obj_index[obj.id]=&obj - r_objects.begin();});
		m_dirty=0;
		}
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	std::for_each(r_objects.begin(),r_objects.end(),[&disp](auto& x)
		{
		if(x.active)
			{x.r_obj->render(disp);}
		});
	}
