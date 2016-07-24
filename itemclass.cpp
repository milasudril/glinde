//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"itemclass.o",
//@	            "type":"object"
//@	        }
//@	    ]
//@	}
#include "itemclass.h"
#include "resourceobject.h"
#include "resourcemanager.h"
#include "datasource.h"
#include "model.h"
#include <cmath>

using namespace Glinde;

ItemClass::ItemClass(ResourceManager& rc,DataSource& source)
	{
	ResourceObject itemclass(source);
	m_name=String( static_cast<const char*>( itemclass.objectGet("name") ) );
	m_mass_density=static_cast<float>(
		static_cast<double> ( itemclass.objectGet("mass_density") ));
	m_mass_dimension=static_cast<float>(
		static_cast<double> ( itemclass.objectGet("mass_dimension") ));

	r_model=&rc.modelGet(source.nameGet(),static_cast<const char*>(itemclass.objectGet("model")));
	}

float ItemClass::massGet() const noexcept
	{
	auto h=r_model->frameGet(0).bounding_box.size().z;
	return m_mass_density*std::pow(h,m_mass_dimension);
	}
