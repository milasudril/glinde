//@	{"targets":[{"name":"init.hpp","type":"include"}]}

#ifndef ANGLE_INIT_HPP
#define ANGLE_INIT_HPP

#include "exceptionhandler.hpp"
#include "contextguard.hpp"
#include <GL/glew.h>

namespace Angle
	{
	template<class ContextProvider>
	void init(ContextProvider&& provider)
		{init(provider);}

	struct Version
		{
		int16_t major;
		int16_t minor;
		
		enum class Profile:int16_t{CORE,COMPATIBILITY,ANY};
		Profile profile;
		bool forward_compatible;
		};

	template<class ContextProvider>
	void init(ContextProvider& provider)
		{
		provider.require(Version{4,5,Version::Profile::CORE,1});
		ContextGuard<decltype(provider.contextUserGet())> guard(provider.contextUserGet());
		glewExperimental=GL_TRUE;
		auto status=glewInit();
		if(status!=GLEW_OK)
			{exceptionRaise(Error("GLEW initialization failed. ",glewGetErrorString(status)));}
		}
	}

#endif
