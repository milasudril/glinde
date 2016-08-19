//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"worlddefault.o",
//@	            "type":"object"
//@	        }
//@	    ]
//@	}
#include "worlddefault.h"
#include "archive.h"
#include "sdk.h"
#include "itemclass.h"
#include "logwriter.h"

using namespace Glinde;

auto WorldDefault::eventHandlerCreate(const ActionProgram& program)->decltype(m_world_eh)
	{
	auto factory=program.entryPointGet<EventHandlerFactory>("Glinde_World_EventHandler_create");
	return decltype(m_world_eh){&factory(logGlobalGet()),EventHandlerDisposer()};
	}


WorldDefault::WorldDefault(const char* filename,MessageQueue& msg_queue):
	 m_fs(new Archive(filename))
	,m_program(*m_fs.get(),"world.so",sdk())
	,m_world_eh(eventHandlerCreate(m_program))
	,m_resources(*m_fs.get()),r_site(nullptr),r_msg_queue(&msg_queue)
	{
	logWrite(Log::MessageType::INFORMATION,"World successfully loaded",{});
	m_world_eh->onLoaded(*this);
	}

WorldDefault::~WorldDefault()
	{
	m_world_eh->onUnload(*this);
	}
