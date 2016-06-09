#ifdef __WAND__
target[name[worldobject.o] type[object]]
#endif

#include "worldobject.h"
#include "item.h"
#include "itemclass.h"
#include "debug.h"

using namespace Glinde;

WorldObject::WorldObject(const Item& item):
	 m_frame_start(0),m_frame_current(0),m_frame_end(0)
	,m_velocity(0.0f,0.0f,0.0f)
	,m_force(0.0f,0.0f,0.0f)
	,m_damping(0.0f)
	,m_charge(0.0f)
	{
	auto& item_class=item.classGet();
	GLINDE_DEBUG_PRINT("Spawning object of class %s",item_class.nameGet());
	modelSet(&item_class.modelGet());
	positionSet(item.positionGet());
	massSet(item_class.massGet());
	}
