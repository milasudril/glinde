//@	{"targets":[{"name":"glinde","type":"application"}]}

#include "engine.hpp"
#include "errormessage.hpp"
#include "timer/timerdummy.hpp"
#include "timer/timerreal.hpp"
#include "log/logwriter.hpp"

#ifdef PROFILE
inline Glinde::TimerDummy timerCreate(double rate)
	{return std::move(Glinde::TimerDummy(rate));}
#else
inline Glinde::TimerReal timerCreate(double rate)
	{return std::move(Glinde::TimerReal(rate));}
#endif


int main()
	{
	try
		{
		Glinde::Engine engine;
		engine.run(timerCreate(30.0));

	//TODO:
	//	check argv
	//		If a world is going to be loaded
	//			Load that world and move input control to its menu
	//		Else
	//			Move input to console.
	//	Console can always be accessed through key X
	//	Esc returns to previous input mode


		}
	catch(const Glinde::ErrorMessage& msg)
		{
		fprintf(stderr,"%s\n",msg.messageGet());
		return -1;
		}

	return 0;
	}
