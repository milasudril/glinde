//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"itemclass.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"itemclass.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
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

			const Model& modelGet() const noexcept
				{return *r_model;}

			float massGet() const noexcept;

		private:
			String m_name;
			const Model* r_model;
			float m_mass_density;
			float m_mass_dimension;
		};
	}

#endif
