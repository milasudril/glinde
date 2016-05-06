#ifdef __WAND__
target[name[texturemanager.o] type[object]]
#endif

#include "texturemanager.h"
#include "filesystem.h"
#include "stringkey.h"
#include "image.h"
#include "errormessage.h"
#include "narrow_cast.h"

using namespace Glinie;

TextureManager::TextureManager(Filesystem& fs):r_fs(fs)
	{}

TextureManager::~TextureManager()
	{}

Image& TextureManager::textureGet(const char* sibling,const char* filename)
	{
	auto filename_full=r_fs.filenameFromSibling(sibling,filename);
	auto ip=m_images.find(Stringkey(filename_full.begin()));
	if(ip!=m_images.end())
		{return ip->second;}

	auto id=narrow_cast<uint32_t>( m_images.size() );

	return m_images.emplace(std::pair<Stringkey,Image>
		{
		 Stringkey(filename_full.begin())
		,Image( r_fs.fileReferenceCreate(filename_full.begin()) , id)
		}).first->second;
	}
