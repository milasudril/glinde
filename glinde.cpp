//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"glinde",
//@	            "type":"application"
//@	        }
//@	    ]
//@	}
#include "errormessage.h"
#include "logwriter.h"
#include "enginedefault.h"
#include "variant.h"
#include "handle.h"
#include "windowgamedefault.h"
#include "usereventhandler.h"
#include "timerreal.h"

#include "messagequeue.h"

using namespace Glinde;


int main()
	{
	deathtrapHandlerActivate();
	logWrite(Log::MessageType::INFORMATION,"Glinde is starting up",{});
	try
		{
		MessageQueue queue;
		Handle<unsigned int,-1,decltype(&logQueueDetach)>
			queue_guard(logQueueAttach(queue),&logQueueDetach);

		EngineDefault e(queue);
		logWrite(Log::MessageType::INFORMATION,"Engine initialized",{});
		UserEventHandler test(e);
		WindowGameDefault mainwin("Glinde",800,600,test);
		TimerReal world_clock(30);
		logWrite(Log::MessageType::INFORMATION
			,"World clock ticks every #0; second",{world_clock.delayGet()});
		e.windowSet(&mainwin).timerSet(&world_clock);
		e.worldLoadAsync("test.zip");
		e.run();
		}
	catch(const ErrorMessage& message)
		{
		logWrite(Log::MessageType::ERROR,"#0;",{message.messageGet()});
		return -1;
		}
	return 0;
	};
