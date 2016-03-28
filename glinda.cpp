#ifdef __WAND__
target[name[glinda] type[application] ]
#endif

#include "errormessage.h"
#include "windowcontroller.h"
#include "windowgame.h"
#include "logwriter.h"
#include "usereventhandler.h"
#include "worldobject.h"
#include "world.h"
#include "renderer.h"
#include "archive.h"

using namespace Glinda;

int main()
	{
	deathtrapHandlerActivate();
	logWrite(LogMessageType::INFORMATION,"Glinda is starting up");
	try
		{
		WorldObject camera;

		camera.locationSet(glm::vec3(0.0f,-2.0f,0.0f));


		World world{Archive("test.zip")};
		UserEventHandler event_handler(camera);

	/*	WindowController window_controller;

		WindowGame mainwin("Glinda",640,480,event_handler,world,camera);

		window_controller.eventWindowSet(&mainwin);
		window_controller.inputLoopRun();
		window_controller.eventWindowSet(nullptr);*/
		}
	catch(const ErrorMessage& message)
		{
		logWrite(LogMessageType::ERROR,"%s",message.messageGet());
		return -1;
		}
	return 0;
	}
