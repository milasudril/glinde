//@	{"targets":[{"name":"camerabuffer.hpp","type":"include"}]}

#ifndef GLINDE_CAMERABUFFER_HPP
#define GLINDE_CAMERABUFFER_HPP

#include "../framebuffercontent.hpp"

namespace Glinde
	{
	typedef FramebufferContent<Angle::TextureFormat::RGB16F
				,Angle::TextureFormat::R32F> CameraBuffer;
	}

#endif
