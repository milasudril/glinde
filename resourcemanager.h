//@	{
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"resourcemanager.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_RESOURCEMANAGER_H
#define GLINDE_RESOURCEMANAGER_H

namespace Glinde
	{
	class Filesystem;
	class Model;
	class Image;
	class ItemClass;
	class Map;
	class Stringkey;
	class World;
	class SiteDefault;

	class ResourceManager
		{
		public:
			virtual const Model& modelGet(const char* sibling_file,const char* filename)=0;
			virtual const ItemClass& itemClassGet(const Stringkey& classname) const=0;
			virtual const Image& textureGet(const char* sibling_file,const char* filename)=0;
			virtual SiteDefault& siteGet(const Stringkey& mapname,World& world_notify)=0;
		};
	}

#endif
