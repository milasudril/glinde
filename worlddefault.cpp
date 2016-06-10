#ifdef __WAND__
target[name[worlddefault.o] type[object]]
#endif

#include "worlddefault.h"
#include "archive.h"
#include "sdk.h"

using namespace Glinde;

static
World::EventHandler& eventHandlerCreate(const ActionProgram& program)
	{
	auto factory=program.entryPointGet<World::EventHandlerFactory>("Glinde_World_EventHandler_create");
	return factory();
	}


WorldDefault::WorldDefault(const char* filename):m_fs(new Archive(filename))
	,m_program(*m_fs.get(),"world.so",sdk())
	,r_world_eh(eventHandlerCreate(m_program))
	,m_resources(*m_fs.get()),r_site(nullptr)
	{
	r_world_eh.onLoaded(*this);
	}

WorldDefault::~WorldDefault()
	{
	r_world_eh.onUnload(*this);
	}
