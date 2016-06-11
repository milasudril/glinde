#ifdef __WAND__
target[name[resourcemanager.h] type[include]]
dependency[resourcemanager.o]
#endif

#ifndef GLINDE_RESOURCEMANAGER_H
#define GLINDE_RESOURCEMANAGER_H

#include "sitedefault.h"
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

	class ResourceManager
		{
		public:
			ResourceManager(Filesystem& source);
			~ResourceManager();

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
