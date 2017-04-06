//@	{"targets":[{"name":"enginebase.hpp","type":"include"}]}

#ifndef GLINDE_ENGINEBASE_HPP
#define GLINDE_ENGINEBASE_HPP

#include <memory>

namespace Glinde
	{
	class Timer;
	class GameLoader;
	class Timeinfo;
	class Game;
	class EngineBase
		{
		public:
			virtual ~EngineBase()=default;
			virtual void stop()=0;
			virtual void run(Timer& timer)=0;
			virtual void consoletest()=0;
			virtual void gameLoad(const char* archive)=0;
			virtual void operator()(const Timeinfo& ti,GameLoader* loader,int x)=0;
			virtual void operator()(const Timeinfo& ti,std::unique_ptr<Game>&& game,int x)=0;
		};
	}

#endif