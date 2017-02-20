//@	{
//@	 "targets":[{"name":"init.o","type":"object","pkgconfig_libs":["glew"]}]
//@ }

#include "init.hpp"
#include "exceptionhandler.hpp"
#include <GL/glew.h>
#include <cstdio>

using namespace Angle;

#ifndef APIENTRY
#define APIENTRY
#endif

void APIENTRY debug_callback(GLenum source,GLenum type
	,GLuint id,GLenum severity,GLsizei length,const GLchar* message
	,const void* userParam)
	{
	fprintf(stderr, "OpenGL: %s\n", message);
	}

VersionResponse Angle::init(bool debug)
	{
	glewExperimental=GL_TRUE;
	auto status=glewInit();
	if(status!=GLEW_OK)
		{
		exceptionRaise(Error("GLEW initialization failed. "
			,reinterpret_cast<const char*>(glewGetErrorString(status))));
		}
	while(glGetError()!=GL_NO_ERROR);
	if(debug)
		{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(debug_callback,NULL);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
		}

	return 
		{
		 reinterpret_cast<const char*>(glGetString(GL_VENDOR))
		,reinterpret_cast<const char*>(glGetString(GL_RENDERER))
		,reinterpret_cast<const char*>(glGetString(GL_VERSION))
		,reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION))
		};
	}

