//@	{"targets":[{"name":"enginebase.hpp","type":"include"}]}

#ifndef GLINDE_ENGINEBASE_HPP
#define GLINDE_ENGINEBASE_HPP

namespace Glinde
	{
	class Timer;
	class EngineBase
		{
		public:
			virtual ~EngineBase()=default;
			virtual void stop()=0;
			virtual void run(Timer& timer)=0;
			virtual void consoletest()=0;
			virtual void gameLoad(const char* archive)=0;
		};
	}

#endif