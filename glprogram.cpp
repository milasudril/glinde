#ifdef __WAND__
target[name[glprogram.o] type[object]]
#endif

#include "glprogram.h"
#include "errormessage.h"

using namespace Glinda;

void GlProgram::link()
	{
	glLinkProgram(id);
	GLint result;
	glGetProgramiv(id,GL_LINK_STATUS,&result);
	if(result==GL_FALSE)
		{
		glGetProgramiv(id,GL_INFO_LOG_LENGTH,&result);
		if(result>0)
			{
			char buffer[512];
			glGetProgramInfoLog(id,512,NULL,buffer);
			throw ErrorMessage("An error occured while trying to link an OpenGL shader program:\n"
				"%s",buffer);
			}
		}
	}
