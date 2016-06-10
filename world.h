#ifdef __WAND__
target[name[world.h] type[include]]
#endif

#ifndef GLINDE_WORLD_H
#define GLINDE_WORLD_H

namespace Glinde
	{
	class Stringkey;

	class World
		{
		public:
			class EventHandler
				{
				public:
					virtual ~EventHandler()=default;
					virtual void onLoaded(World& world)=0;
					virtual void onUnload(World& world)=0;
				};
			virtual ~World()=default;
			virtual World& siteSet(const Stringkey& key)=0;
			virtual World& playerCreate(const Stringkey& classname)=0;

		typedef EventHandler& (*EventHandlerFactory)();
		};
	}

extern "C"
	{
	Glinde::World::EventHandler& Glinde_World_EventHandler_create();
	}

#endif
