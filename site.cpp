#ifdef __WAND__
target[name[site.o] type[object]]
#endif

#include "site.h"
#include "map.h"
#include "debug.h"

using namespace Glinde;

Site::Site(const Map& map):m_tree(map.modelGet(),0)
	{
 	GLINDE_DEBUG_PRINT("Building site from map %s",map.nameGet());
	r_model=&map.modelGet();
	auto items=map.itemsGet();
	auto ptr=items.begin();
	auto ptr_end=items.end();
	while(ptr!=ptr_end)
		{
		m_objects.insert(WorldObject(*ptr));
		++ptr;
		}
	}
