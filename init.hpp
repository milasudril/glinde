//@	{"targets":[{"name":"init.hpp","type":"include"}]}

#ifndef ANGLE_INIT_HPP
#define ANGLE_INIT_HPP

#include "exceptionhandler.hpp"
#include "contextguard.hpp"
#include <GL/glew.h>

namespace Angle
	{
	struct VersionRequest
		{
		int16_t major;
		int16_t minor;
		
		enum class Profile:int16_t{CORE,COMPAT,ANY};
		Profile profile;
		bool forward_compatible;
		};

	static constexpr VersionRequest gl_version_requirements()
		{return VersionRequest{4,5,VersionRequest::Profile::CORE,1};}

	struct VersionResponse
		{
		const char* vendor;
		const char* renderer;
		const char* version;
		const char* glsl_version;
		};

	VersionResponse init()
		{
		glewExperimental=GL_TRUE;
		auto status=glewInit();
		if(status!=GLEW_OK)
			{
			exceptionRaise(Error("GLEW initialization failed. "
				,reinterpret_cast<const char*>(glewGetErrorString(status))));
			}

		return 
			{
			 reinterpret_cast<const char*>(glGetString(GL_VENDOR))
			,reinterpret_cast<const char*>(glGetString(GL_RENDERER))
			,reinterpret_cast<const char*>(glGetString(GL_VERSION))
			,reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION))
			};
		}
	}
#endif
