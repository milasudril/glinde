#ifdef __WAND__
target[name[glinda] type[application] ]
#endif

#include "errormessage.h"
#include "windowsystem.h"
#include "windowgame.h"
#include "logwriter.h"
#include "usereventhandler.h"
#include "worldobject.h"
#include "world.h"
#include "renderer.h"

using namespace Glinda;

int main()
	{
	deathtrapHandlerActivate();
	logWrite(LogMessageType::INFORMATION,"Glinda is starting up");
	try
		{
		WorldObject camera;

		camera.locationSet(glm::vec3(0.0f,-2.0f,0.0f));

		World world;
		UserEventHandler event_handler(camera);

		WindowSystem window_system;

		WindowGame mainwin("Glinda",640,480,event_handler,world,camera);

		window_system.eventWindowSet(&mainwin);
		window_system.inputLoopRun();
		window_system.eventWindowSet(nullptr);
		}
	catch(const ErrorMessage& message)
		{
		logWrite(LogMessageType::ERROR,"%s",message.messageGet());
		return -1;
		}
	return 0;
	}
