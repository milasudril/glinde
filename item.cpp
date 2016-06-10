#ifdef __WAND__
target[name[item.o] type[object]]
#endif

#include "item.h"
#include "resourceobject.h"
#include "resourcemanager.h"
#include "errormessage.h"
#include "stringkey.h"

using namespace Glinde;

Item::Item(ResourceManager& rc,const Glinde::ResourceObject& item)
	{
	m_rot_z=static_cast<float>(static_cast<double>(item.objectGet("rot_z")));
	m_tag=String(static_cast<const char*>(item.objectGet("tag")));

		{
		auto position_init=item.objectGet("position_init");
		auto N=position_init.objectCountGet();
		if(N!=3)
			{
			throw ErrorMessage("Error loading an item from the map definition: "
				"The number of coordinates has to be equal to three.");
			}

		for(int k=0;k<3;++k)
			{
			m_position[k]=static_cast<float>(static_cast<double>(position_init.objectGet(k)));
			}
		}

	r_class=nullptr;
	if(item.objectExists("class"))
		{
		r_class=&rc.itemClassGet(Stringkey(static_cast<const char*>(item.objectGet("class"))));
		}
	}
