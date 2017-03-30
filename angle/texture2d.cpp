//@	{"targets":[{"name":"texture2d.o","type":"object","pkgconfig_libs":["glew"]}]}

#include "texture2d.hpp"

using namespace Angle;

Texture2D::Texture2D(TextureFormat format,GLsizei width,GLsizei height)
	{
	glCreateTextures(GL_TEXTURE_2D,1,&m_handle);
	glTextureStorage2D(m_handle,levels(width,height),native_type(format),width,height);
	auto error=glGetError();
	if(error!=GL_NO_ERROR)
		{
		glDeleteTextures(1,&m_handle);
		exceptionRaise(Error("Failed to allocate texture storage."));
		}
	m_format=format;
	m_width=width;
	m_height=height;
	m_unit=static_cast<GLuint>(-1);
	}

void Texture2D::realloc(GLsizei width_in,GLsizei height_in)
	{
	GLuint handle;
	glCreateTextures(GL_TEXTURE_2D,1,&handle);
	glTextureStorage2D(handle,levels(width_in,height_in),native_type(m_format),width_in,height_in);
	auto error=glGetError();
	if(error!=GL_NO_ERROR)
		{
		glDeleteTextures(1,&handle);
		exceptionRaise(Error("Failed to allocate texture storage."));
		}
	glFinish();
	if(m_unit!=static_cast<GLuint>(-1))
		{glBindTextureUnit(m_unit,0);}
	glDeleteTextures(1,&m_handle);
	if(m_unit!=static_cast<GLuint>(-1))
		{glBindTextureUnit(m_unit,handle);}
	m_handle=handle;
	m_width=width_in;
	m_height=height_in;
	}
