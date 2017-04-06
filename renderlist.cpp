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
//@	"targets":[{"name":"renderlist.o","type":"object"}]
//@	}

#include "renderlist.hpp"
#include "display.hpp"
#include "renderobject.hpp"
#include "time/timeinfo.hpp"
#include <algorithm>

using namespace Glinde;

RenderList::RenderList():m_dirty(1)
	{
	glEnable(GL_BLEND);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.216f,0.216f,0.216f,1.0f);
	}

void RenderList::framebufferResize(int width,int height)
	{
	glViewport(0,0,width,height);
	std::for_each(r_objects.begin(),r_objects.end(),[width,height](auto& x)
		{x.r_obj->framebufferResize(width,height);});
	}

void RenderList::render(Display& disp,const Timeinfo& ti) const noexcept
	{
	if(m_dirty)
		{
		std::sort(r_objects.begin(),r_objects.end(),[](const auto& a,const auto& b)
			{return a.index<b.index;});
	//	Re-assign object indices
		std::for_each(r_objects.begin(),r_objects.end(),[this](const auto& obj)
			{m_id_to_obj_index[obj.id]=static_cast<uint32_t>(&obj - r_objects.begin());});
		m_dirty=0;
		}
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	std::for_each(r_objects.begin(),r_objects.end(),[&disp,ti](auto& x)
		{
		if(x.active)
			{x.r_obj->render(disp,ti);}
		});
	}
