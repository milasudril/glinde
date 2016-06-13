#ifdef __WAND__
target[name[map.o] type [object]]
#endif

#include "map.h"
#include "resourceobject.h"
#include "datasource.h"
#include "resourcemanager.h"
#include <algorithm>

using namespace Glinde;

namespace
	{
	class Sort
		{
		public:
			bool operator()(const Item& a,const Item& b)
				{
				return a.tagGet() < b.tagGet();
				}
		};

	class Find
		{
		public:
			bool operator()(const Item& a,const Stringkey& tag)
				{
				return a.tagGet() < tag;
				}
		};
	}

const Item* Map::itemFind(const Range<const Item>& items,const Stringkey& tag)
	{
	auto i=std::lower_bound(items.begin(),items.end(),tag,Find());
	if(i==items.end())
		{return nullptr;}
	return Find()(*i,tag)?
		nullptr : i;
	}

Map::Map(ResourceManager& rc,DataSource& source):m_id("")
	{
	ResourceObject obj(source);

	m_name=String(static_cast<const char*>(obj.objectGet("name")));
	m_id=Stringkey(m_name.begin());
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

	std::sort(m_items_init.begin(),m_items_init.end(),Sort());
	}
