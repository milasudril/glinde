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
