#ifdef __WAND__
target[name[gltexture.o] type[object]]
#endif

#include "gltexture.h"
#include "image.h"
#include "debug.h"

using namespace Glinda;

void GlTexture::dataSet(const Image& source) noexcept
	{
/*	GLINDA_DEBUG_PRINT("Upload texture data %u x %u"
		,source.widthGet(),source.heightGet());*/
	glBindTexture(GL_TEXTURE_2D,id);
	switch(source.channelCountGet())
		{
		case 1:
			glTexImage2D(GL_TEXTURE_2D,0,GL_RED
				,source.widthGet(),source.heightGet()
				,0
				,GL_RED
				,GL_HALF_FLOAT
				,source.pixelsGet());
			break;

		case 3:
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGB
				,source.widthGet(),source.heightGet()
				,0
				,GL_RGB
				,GL_HALF_FLOAT
				,source.pixelsGet());

		case 4:
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA
				,source.widthGet(),source.heightGet()
				,0
				,GL_RGBA
				,GL_HALF_FLOAT
				,source.pixelsGet());
		}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
