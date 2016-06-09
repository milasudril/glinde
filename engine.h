#ifdef __WAND__
target[name[engine.h] type[include]]
#endif

#ifndef GLINDE_ENGINE_H
#define GLINDE_ENGINE_H

#include <cstdint>

namespace Glinde
	{
	class World;
	class Engine
		{
		public:
			class EventHandler
				{
				public:
					virtual void onWorldLoaded(Engine& engine,World& world)=0;
				};

			virtual void stop()=0;
			virtual void consoleWrite(const char* message)=0;
			virtual void consoleWrite(char ch)=0;
			virtual void consoleColorMaskSet(uint8_t colormask)=0;
		};
	}

#endif
