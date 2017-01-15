//@	{"targets":[{"name":"init.hpp","type":"include"}]}

#ifndef ANGLE_INIT_HPP
#define ANGLE_INIT_HPP

#include "exceptionhandler.hpp"
#include "contextguard.hpp"
#include <GL/glew.h>

namespace Angle
	{
	template<class ContextUser>
	void init(ContextUser&& user)
		{init(user);}

	struct Version
		{
		int16_t major;
		int16_t minor;
		
		enum class Profile:int16_t{CORE,COMPAT,ANY};
		Profile profile;
		bool forward_compatible;
		};

	static constexpr Version version_requirements{4,5,Version::Profile::CORE,1};

	template<class ContextUser>
	void init(ContextUser& user)
		{
		ContextGuard<ContextUser> guard(user);
		glewExperimental=GL_TRUE;
		auto status=glewInit();
		if(status!=GLEW_OK)
			{exceptionRaise(Error("GLEW initialization failed. ",glewGetErrorString(status)));}
		}
	}

#endif
