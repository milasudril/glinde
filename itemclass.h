#ifdef __WAND__
target[name[itemclass.h] type[include]]
dependency[itemclass.o]
#endif

#ifndef GLINDE_ITEMCLASS_H
#define GLINDE_ITEMCLASS_H

#include "string.h"


namespace Glinde
	{
	class ResourceManager;
	class DataSource;
	class Model;

	class ItemClass
		{
		public:
			ItemClass()=default;

			explicit ItemClass(ResourceManager& rc,DataSource& source);
			explicit ItemClass(ResourceManager& rc,DataSource&& source):
				ItemClass(rc,source)
				{}

			const char* nameGet() const noexcept
				{return m_name.begin();}

			float massGet() const noexcept;

		private:
			String m_name;
			const Model* r_model;
			float m_mass_density;
			float m_mass_dimension;
		};
	}

#endif
