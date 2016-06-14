#ifdef __WAND__
target[name[gltexture.o] type[object]]
#endif

#include "gltexture.h"
#include "image.h"
#include "debug.h"
#include "variant.h"

using namespace Glinde;

void GlTexture::dataSet(const Image& source) noexcept
	{
	GLINDE_DEBUG_PRINT("Upload texture data #0; x #1;"
		,source.widthGet(),source.heightGet());
	glBindTexture(GL_TEXTURE_2D,m_id);
	switch(source.channelCountGet())
		{
		case 1:
			glTexImage2D(GL_TEXTURE_2D,0,GL_R16F
				,source.widthGet(),source.heightGet()
				,0
				,GL_RED
				,GL_HALF_FLOAT
				,source.pixelsGet());
			break;

		case 3:
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGB16F
				,source.widthGet(),source.heightGet()
				,0
				,GL_RGB
				,GL_HALF_FLOAT
				,source.pixelsGet());

		case 4:
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F
				,source.widthGet(),source.heightGet()
				,0
				,GL_RGBA
				,GL_HALF_FLOAT
				,source.pixelsGet());
		}
	filterEnable();
	}
