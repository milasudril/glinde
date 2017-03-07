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

	inline const uint16_t* native_type(const FaceIndirect* x) noexcept
		{return reinterpret_cast<const uint16_t*>(x);}
	}


#endif
