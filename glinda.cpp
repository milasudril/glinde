#ifdef __WAND__
target[name[glinda] type[application] ]
#endif

#include "errormessage.h"
#include "uimanager.h"
#include "windowgame.h"
#include "logwriter.h"
#include "usereventhandler.h"
#include "worldobject.h"
#include "world.h"
#include "renderer.h"
#include "archive.h"
#include "timer.h"

using namespace Glinda;

int main()
	{
	deathtrapHandlerActivate();
	logWrite(LogMessageType::INFORMATION,"Glinda is starting up");
	try
		{
		WorldObject player;
		Timer world_clock(60.0);
		logWrite(LogMessageType::INFORMATION
			,"World clock ticks every %.15g second",world_clock.delayGet());

	//	Archive src{"test.zip"};
		Archive src{"test (kopiera 1).zip"};
		World world{src};
		UserEventHandler event_handler(player,world_clock.delayGet());

		UIManager window_controller;

		WindowGame mainwin("Glinda",640,480,event_handler,world,player);

		window_controller.eventWindowSet(&mainwin);
		window_controller.inputLoopRun(world_clock);
		window_controller.eventWindowSet(nullptr);
		}
	catch(const ErrorMessage& message)
		{
		logWrite(LogMessageType::ERROR,"%s",message.messageGet());
		return -1;
		}
	return 0;
	}
