//@	{
//@	"targets":[{"name":"renderlist.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"renderlist.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_RENDERLIST_HPP
#define GLINDE_RENDERLIST_HPP

#include "arraydynamic.hpp"

namespace Glinde
	{
	class Display;
	class RenderObject;

	class RenderList
		{
		public:
			

			RenderList();
			void framebufferResize(int width,int height);
			void render(Display& disp) const noexcept;
			void objectPush(RenderObject& obj,unsigned int z_index);

		private:
		};
	}

#endif
