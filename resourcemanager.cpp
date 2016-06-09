#ifdef __WAND__
target[name[resourcemanager.o] type [object]]
#endif

#include "resourcemanager.h"
#include "stringkey.h"
#include "itemclass.h"
#include "map.h"
#include "image.h"
#include "model.h"
#include "filesystem.h"
#include "site.h"
#include "resourceobject.h"
#include "errormessage.h"
#include "narrow_cast.h"

using namespace Glinde;

ResourceManager::ResourceManager(Filesystem& source):r_source(source)
	{
	ResourceObject world(source.fileReferenceCreate("index.json"));

		{
		auto classes=world.objectGet("item_classes");
		auto N_classes=classes.objectCountGet();
		for(decltype(N_classes) k=0; k<N_classes; ++k)
			{
			auto obj=classes.objectGet(k);
			ItemClass ic(*this
				,source.fileReferenceCreate(static_cast<const char*>(obj)));
			auto key=Stringkey(ic.nameGet());
			m_classes[key]=std::move(ic);
			}
		}

		{
		auto maps=world.objectGet("maps");
		auto N_maps=maps.objectCountGet();
		for(decltype(N_maps) k=0; k<N_maps; ++k)
			{
			auto obj=maps.objectGet(k);
			Map map(*this
				,source.fileReferenceCreate(static_cast<const char*>(obj)));
			auto key=Stringkey(map.nameGet());
			m_maps[key]=std::move(map);
			}
		}
	}

ResourceManager::~ResourceManager()
	{}


const ItemClass& ResourceManager::itemClassGet(const Stringkey& classname) const
	{
	auto i=m_classes.find(classname);
	if(i==m_classes.end())
		{throw ErrorMessage("Item class has not been loaded.");}
	return i->second;
	}

const Model& ResourceManager::modelGet(const char* sibling_file,const char* filename)
	{
	auto filename_full=r_source.filenameFromSibling(sibling_file,filename);
	auto ip=m_models.find(Stringkey(filename_full.begin()));
	if(ip!=m_models.end())
		{return ip->second;}

	return m_models.emplace(std::pair<Stringkey,Model>
		{
		 Stringkey(filename_full.begin())
		,Model(*this,r_source.fileReferenceCreate(filename_full.begin()) )
		}).first->second;
	}

const Image& ResourceManager::textureGet(const char* sibling,const char* filename)
	{
	auto filename_full=r_source.filenameFromSibling(sibling,filename);
	auto ip=m_textures.find(Stringkey(filename_full.begin()));
	if(ip!=m_textures.end())
		{return ip->second;}

	auto id=narrow_cast<uint32_t>( m_textures.size() );

	return m_textures.emplace(std::pair<Stringkey,Image>
		{
		 Stringkey(filename_full.begin())
		,Image( r_source.fileReferenceCreate(filename_full.begin()) , id)
		}).first->second;
	}

Site& ResourceManager::siteGet(const Stringkey& mapname)
	{
	auto i_site=m_sites.find(mapname);
	if(i_site==m_sites.end())
		{
		auto i=m_maps.find(mapname);
		if(i==m_maps.end())
			{throw ErrorMessage("There is no map for the given site");}

		return m_sites.emplace(std::pair<Stringkey,Site>
			{mapname,Site(i->second)}).first->second;
		}
	return i_site->second;
	}
