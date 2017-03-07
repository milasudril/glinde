//@	{"targets":[{"name":"glinde","type":"application"}]}

#include "engine.hpp"
#include "errormessage.hpp"
#include "timerdummy.hpp"
#include "timerreal.hpp"
#include "logwriter.hpp"

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
		}
	catch(const Glinde::ErrorMessage& msg)
		{
		fprintf(stderr,"%s\n",msg.messageGet());
		return -1;
		}

	return 0;
	}
