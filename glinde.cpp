#ifdef __WAND__
target[name[glinde] type[application] ]
#endif

#include "errormessage.h"
#include "uimanager.h"
#include "windowgame.h"
#include "logwriter.h"
#include "usereventhandler.h"
#include "worldobject.h"
#include "worlddefault.h"
#include "renderer.h"
#include "archive.h"
#include "timerreal.h"
#include "gameloop.h"

using namespace Glinde;

int main()
	{
	deathtrapHandlerActivate();
	logWrite(LogMessageType::INFORMATION,"Glinde is starting up");
	try
		{
		Archive src{"test.zip"};
	//	World world{src};
	//	world.siteSet(Stringkey("map00"));

	/*	UserEventHandler event_handler(world);
		UIManager ui;
		WindowGame mainwin("Glinde",640,480,event_handler,world);
		Timer world_clock(30.0);
		logWrite(LogMessageType::INFORMATION
			,"World clock ticks every %.15g second",world_clock.delayGet());
		gameLoopRun(ui,mainwin,world_clock,world);*/
		}
	catch(const ErrorMessage& message)
		{
		logWrite(LogMessageType::ERROR,"%s",message.messageGet());
		return -1;
		}
	return 0;
	}
