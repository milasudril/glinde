#ifdef __WAND__
target[name[itemclass.o] type[object]]
#endif

#include "itemclass.h"
#include "resourceobject.h"
#include "resourcemanager.h"
#include "datasource.h"

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
