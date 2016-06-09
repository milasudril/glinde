#ifdef __WAND__
target[name[map.h] type[include]]
dependency[map.o]
#endif

#ifndef GLINDE_MAP_H
#define GLINDE_MAP_H

#include "string.h"
#include "item.h"
#include "arraysimple.h"

namespace Glinde
	{
	class ResourceManager;
	class DataSource;
	class Model;

	class Map
		{
		public:
			Map(){}

			explicit Map(ResourceManager& rc,DataSource& source);
			explicit Map(ResourceManager& rc,DataSource&& source):Map(rc,source)
				{}

			const char* nameGet() const noexcept
				{return m_name.begin();}

			const char* titleGet() const noexcept
				{return m_title.begin();}

			const Model& modelGet() const noexcept
				{return *r_model;}

			Range<const Item> itemsGet() const noexcept
				{return m_items_init;}

		private:
			String m_name;
			String m_title;
			const Model* r_model;
			ArraySimple<Item> m_items_init;
		};
	}

#endif
