//@	{
//@	"targets":[{"name":"imagerenderer.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"imagerenderer.o","rel":"implementation"}]
//@ }

#ifndef GLINDE_IMAGERENDERER_HPP
#define GLINDE_IMAGERENDERER_HPP

#include "angle/texture2d.hpp"

namespace Glinde
	{
	class ImageRenderer
		{
		public:
			ImageRenderer();

			~ImageRenderer();

			void render(const Angle::Texture2D& texture,bool mirror_x,bool mirror_y) const noexcept;
		};
	}

#endif
