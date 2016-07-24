//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"texturemanager.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"texturemanager.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <map>

namespace Glinde
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
