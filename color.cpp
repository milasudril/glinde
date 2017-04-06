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
//@	{"targets":[{"type":"object","name":"color.o"}]}

#include "color.hpp"

using namespace Glinde;

static constexpr ArrayFixed<float,256> srgb_lut_generate() noexcept
	{
	ArrayFixed<float,256> ret;
	for(int k=0;k<256;++k)
		{ret[k]=Color::fromSRGB(static_cast<float>(k)/255.0f);}
	return ret;
	}

const ArrayFixed<float,256> Color::srgb_lut=srgb_lut_generate();
