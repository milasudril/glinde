//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"map.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"map.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_MAP_H
#define GLINDE_MAP_H

#include "string.h"
#include "item.h"
#include "arraysimple.h"
#include "stringkey.h"
#include <cassert>

namespace Glinde
	{
	class ResourceManager;
	class DataSource;
	class Model;

	class Map
		{
		public:
			Map():m_id(""){}

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

			const Stringkey& idGet() const noexcept
				{return m_id;}

			const Item* itemFind(const Stringkey& tag) const noexcept
				{
				return itemFind(m_items_init,tag);
				}

		private:
			static const Item* itemFind(const Range<const Item>& items
				,const Stringkey& tag);

			Stringkey m_id;
			String m_name;
			String m_title;
			const Model* r_model;
			ArraySimple<Item> m_items_init;
		};
	}

#endif
