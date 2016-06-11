#ifdef __WAND__
target[name[worlddefault.o] type[object]]
#endif

#include "worlddefault.h"
#include "archive.h"
#include "sdk.h"
#include "itemclass.h"

using namespace Glinde;


auto WorldDefault::eventHandlerCreate(const ActionProgram& program)->decltype(m_world_eh)
	{
	auto factory=program.entryPointGet<EventHandlerFactory>("Glinde_World_EventHandler_create");
	return decltype(m_world_eh){&factory(),EventHandlerDisposer()};
	}


WorldDefault::WorldDefault(const char* filename):m_fs(new Archive(filename))
	,m_program(*m_fs.get(),"world.so",sdk())
	,m_world_eh(eventHandlerCreate(m_program))
	,m_resources(*m_fs.get()),r_site(nullptr)
	{
	m_world_eh->onLoaded(*this);
	}

WorldDefault::~WorldDefault()
	{
	m_world_eh->onUnload(*this);
	}

WorldDefault& WorldDefault::playerCreate(const Stringkey& classname)
	{
	const auto& item_class=m_resources.itemClassGet(classname);
	m_player.modelSet(&item_class.modelGet()).massSet(item_class.massGet());
	return *this;
	}
