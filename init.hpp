//@	{
//@	 "targets":[{"name":"init.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"init.o","rel":"implementation"}]
//@ }

#ifndef ANGLE_INIT_HPP
#define ANGLE_INIT_HPP

#include <cstdint>

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

	VersionResponse init();

	}
#endif
