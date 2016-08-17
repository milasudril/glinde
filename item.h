//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"item.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"item.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_ITEM_H
#define GLINDE_ITEM_H

#include "stringkey.h"
#include <glm/vec3.hpp>

namespace Glinde
	{
	class ItemClass;
	class ResourceObject;
	class ResourceManager;

	class Item
		{
		public:
			Item()=default;
			Item(ResourceManager& rc,const Glinde::ResourceObject& item);

			const ItemClass* classGet() const noexcept
				{return r_class;}

			const Stringkey& tagGet() const noexcept
				{return m_tag;}

			const glm::vec3& positionGet() const noexcept
				{return m_position;}

			float rotZGet() const noexcept
				{return m_rot_z;}

			Item& classSet(ResourceManager& rc,const Stringkey& classname);

		private:
			const ItemClass* r_class;
			Stringkey m_tag;
			glm::vec3 m_position;
			float m_rot_z;
		};
	}

#endif
