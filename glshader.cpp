//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"glshader.o",
//@	            "type":"object"
//@	        }
//@	    ]
//@	}
#include "glshader.h"
#include "errormessage.h"
#include "variant.h"

using namespace Glinde;

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
				"#0;"
				,{buffer});
			}
		}
	}
