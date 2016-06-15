#ifdef __WAND__
target[name[glframebuffer.o] type[object]]
#endif

#include "glframebuffer.h"
#include "errormessage.h"
#include "variant.h"

using namespace Glinde;

void GlFramebuffer::targetSet(GlTexture& texture,unsigned int width,unsigned int height)
	{
	m_width=width;
	m_height=height;
	glBindFramebuffer(GL_FRAMEBUFFER,m_handle);
	texture.dataSet(width,height);
	texture.filterDisable();

	glBindRenderbuffer(GL_RENDERBUFFER, m_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth);

	glFramebufferTexture(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,texture.m_id,0);
	GLenum buffer=GL_COLOR_ATTACHMENT0;
	glDisable(GL_DEPTH_TEST);
	glDrawBuffers(1,&buffer);

	auto res=glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(res!=GL_FRAMEBUFFER_COMPLETE)
		{
		throw ErrorMessage("Failed creating a framebuffer: #0;",{res});
		}
	}
