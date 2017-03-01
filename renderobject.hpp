//@	{
//@	"targets":[{"name":"renderobject.hpp","type":"include"}]
//@	}

#ifndef GLINDE_RENDEROBJECT_HPP
#define GLINDE_RENDEROBJECT_HPP

namespace Glinde
	{
	class Display;

	class RenderObject
		{
		public:
			virtual void framebufferResize(int width,int height)=0;
			virtual void render(Display& disp)=0;
		};
	}

#endif
