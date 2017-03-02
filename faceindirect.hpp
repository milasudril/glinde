//@	{"targets":[{"name":"faceindirect.hpp","type":"include"}]}

#ifndef GLINDE_FACEINDIRECT_HPP
#define GLINDE_FACEINDIRECT_HPP

#include "arrayfixed.hpp"
#include <cstdint>

namespace Glinde
	{
	struct FaceIndirect:public ArrayFixed<uint16_t,3>
		{
		using ArrayFixed<uint16_t,3>::ArrayFixed;
		};
	}


#endif
