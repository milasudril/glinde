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
//@	"targets":[{"name":"renderobject.hpp","type":"include"}]
//@	}

#ifndef GLINDE_RENDEROBJECT_HPP
#define GLINDE_RENDEROBJECT_HPP

namespace Glinde
	{
	class Display;
	class Timeinfo;

	class RenderObject
		{
		public:
			virtual void framebufferResize(int width,int height)=0;
			virtual void render(Display& disp,const Timeinfo& ti) const noexcept=0;
		};
	}

#endif
