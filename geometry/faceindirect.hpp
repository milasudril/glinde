//@	{"targets":[{"name":"faceindirect.hpp","type":"include"}]}

#ifndef GLINDE_FACEINDIRECT_HPP
#define GLINDE_FACEINDIRECT_HPP

#include "../storage/arrayfixed.hpp"
#include <cstdint>

namespace Glinde
	{
	template<class T=uint16_t>
	struct FaceIndirect:public ArrayFixed<T,3>
		{
		using ArrayFixed<T,3>::ArrayFixed;
		};

	template<class T>
	inline const T* native_type(const FaceIndirect<T>* x) noexcept
		{return reinterpret_cast<const T*>(x);}
	}


#endif
