//@	{
//@	 "targets":[{"name":"init.o","type":"object","pkgconfig_libs":["glew"]}]
//@ }

#include "init.hpp"
#include "exceptionhandler.hpp"
#include <GL/glew.h>

using namespace Angle;

VersionResponse Angle::init()
	{
	glewExperimental=GL_TRUE;
	auto status=glewInit();
	if(status!=GLEW_OK)
		{
		exceptionRaise(Error("GLEW initialization failed. "
			,reinterpret_cast<const char*>(glewGetErrorString(status))));
		}
	while(glGetError()!=GL_NO_ERROR);
	return 
		{
		 reinterpret_cast<const char*>(glGetString(GL_VENDOR))
		,reinterpret_cast<const char*>(glGetString(GL_RENDERER))
		,reinterpret_cast<const char*>(glGetString(GL_VERSION))
		,reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION))
		};
	}
