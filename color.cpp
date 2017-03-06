//@	{"targets":[{"type":"object","name":"color.o"}]}

#include "color.hpp"

using namespace Glinde;

static constexpr ArrayFixed<float,256> srgb_lut_generate() noexcept
	{
	ArrayFixed<float,256> ret;
	for(int k=0;k<256;++k)
		{ret[k]=Color::fromSRGB(k/255.0f);}
	return ret;
	}

const ArrayFixed<float,256> Color::srgb_lut=srgb_lut_generate();
