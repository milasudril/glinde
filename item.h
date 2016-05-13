#ifdef __WAND__
target[name[item.h] type[include]]
dependency[item.o]
#endif

#ifndef GLINDE_ITEM_H
#define GLINDE_ITEM_H

#include "string.h"
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

		private:
			const ItemClass* r_class;
			String m_tag;
			glm::vec3 m_position;
			float m_rot_z;
		};
	}

#endif
