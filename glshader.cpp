#ifdef __WAND__
target[name[glshader.o] type[object]]
#endif

#include "glshader.h"
#include "errormessage.h"

using namespace Glinie;

void GlShader::compile()
	{
	glCompileShader(id);
	GLint result;
	glGetShaderiv(id,GL_COMPILE_STATUS,&result);
	if(result==GL_FALSE)
		{
		glGetShaderiv(id,GL_INFO_LOG_LENGTH,&result);
		if(result>0)
			{
			char buffer[512];
			glGetShaderInfoLog(id,512,NULL,buffer);
			throw ErrorMessage("An error occured while trying to compile an OpenGL shader:\n"
				"%s"
				,buffer);
			}
		}
	}
