#ifdef __WAND__
target[name[map.o] type [object]]
#endif

#include "map.h"
#include "resourceobject.h"
#include "datasource.h"
#include "resourcemanager.h"

using namespace Glinde;

Map::Map(ResourceManager& rc,DataSource& source)
	{
	ResourceObject obj(source);

	m_name=String(static_cast<const char*>(obj.objectGet("name")));
	m_title=String(static_cast<const char*>(obj.objectGet("title")));
	r_model=&rc.modelGet(source.nameGet()
		,static_cast<const char*>(obj.objectGet("model")));

	auto items=obj.objectGet("items_init");
	auto N_items=items.objectCountGet();
	m_items_init=ArraySimple<Item>(N_items,[&items,&rc](size_t k)
		{
		auto item=items.objectGet(k);
		return Item(rc,item);
		});
	}
