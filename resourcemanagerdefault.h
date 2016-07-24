//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"resourcemanagerdefault.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"resourcemanagerdefault.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_RESOURCEMANAGERDEFAULT_H
#define GLINDE_RESOURCEMANAGERDEFAULT_H

#include "resourcemanager.h"
#include <map>

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

	class ResourceManagerDefault:public ResourceManager
		{
		public:
			ResourceManagerDefault(Filesystem& source);
			~ResourceManagerDefault();

			const Model& modelGet(const char* sibling_file,const char* filename);
			const ItemClass& itemClassGet(const Stringkey& classname) const;
			const Image& textureGet(const char* sibling_file,const char* filename);
			SiteDefault& siteGet(const Stringkey& mapname,World& world_notify);

		private:
			Filesystem& r_source;
			std::map<Stringkey,Model> m_models;
			std::map<Stringkey,Image> m_textures;
			std::map<Stringkey,ItemClass> m_classes;
			std::map<Stringkey,Map> m_maps;
			std::map<Stringkey,SiteDefault> m_sites;
		};
	}

#endif
