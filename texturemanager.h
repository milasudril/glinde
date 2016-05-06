#ifdef __WAND__
target[name[texturemanager.h] type[include]]
dependency[texturemanager.o]
#endif

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <map>

namespace Glinie
	{
	class Image;
	class Filesystem;
	class Stringkey;

	class TextureManager
		{
		public:
			explicit TextureManager(Filesystem& fs);
			~TextureManager();

			Image& textureGet(const char* sibling,const char* filename);


		private:
			std::map<Stringkey,Image> m_images;
			Filesystem& r_fs;
		};
	}

#endif
