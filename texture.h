#ifdef __WAND__
target[name[texture.h] type[include]]
dependency[texture.o]
#endif

#ifndef ANGLE_TEXTURE_H
#define ANGLE_TEXTURE_H

#include <GL/glew.h>

namespace Angle
	{
	class Texture
		{
		public:
			Texture()
				{glGenTextures(1,&handle);}

			void bind(GLenum target)
				{glBindTexture(target,handle);}

			~Texture()
				{glDeleteTextures(1,&handle);}

		private:
			GLuint handle;
		};
	}

#endif
