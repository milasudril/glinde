//@	{
//@	"targets":[{"name":"eventhandler.o","type":"object"}]
//@	,"author":["Torbjörn Rathsman"]
//@	,"year":2016
//@	,"license":"Public domain"
//@	,"description":"Implementation of event handler for Map00"
//@	}

#include "eventhandler.h"
#include "../common/syslog.h"
#include <glinde/stringkey.h>
#include <glinde/log.h>
#include <glinde/variant.h>

using namespace Map00;
using namespace Glinde;

namespace
	{
	class ItemsSpawn:public Site::SpotVisitor
		{
		public:
			void spotVisit(Site& site);
		};
	}

void EventHandler::onInit(Site& site)
	{
	auto tid=site.itemSpawnSync(Stringkey("player_00"),Stringkey("human"));
	g_syslog->write(Log::MessageType::INFORMATION,"Spawned player_00 as tid #0;"
		,{tid});
	site.playerSet(tid);
	}
