//@	{
//@	"targets":[{"name":"eventhandler.o","type":"object"}]
//@	,"author":["Torbjörn Rathsman"]
//@	,"year":2016
//@	,"license":"Public domain"
//@	,"description":"Implementation of event handler for Map00"
//@	}

#include "eventhandler.h"
#include <glinde/stringkey.h>

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
	site.itemSpawn(Stringkey("player_00"),Stringkey("human"));
	}
