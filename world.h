//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"world.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_WORLD_H
#define GLINDE_WORLD_H

namespace Glinde
	{
	class Stringkey;
	class Site;
	class Log;

	class World
		{
		public:
			class EventHandler
				{
				public:
					virtual ~EventHandler()=default;
					virtual void destroy() noexcept=0;
					virtual void onLoaded(World& world)=0;
					virtual void onUnload(World& world){}
					virtual void onSiteCreated(World& world,Site& site)=0;
					virtual void onSiteDestroy(World& world,Site& site) noexcept=0;
					virtual void onSiteMoved(World& world,Site& site) noexcept
						{}
				};
			virtual ~World()=default;
			virtual World& siteSet(const Stringkey& key)=0;
			virtual void siteCreated(Site& site)=0;
			virtual void siteDestroying(Site& site) noexcept=0;
			virtual void siteMoved(Site& site) noexcept=0;

		typedef EventHandler& (*EventHandlerFactory)(Log& log);

		class EventHandlerDisposer
			{
			public:
				void operator()(EventHandler* e) const noexcept
					{e->destroy();}
			};
		};
	}

extern "C"
	{
#if _WIN32 || _WIN64
	#define EXPORT __declspec(dllexport)
#else
	#define EXPORT
#endif
	EXPORT Glinde::World::EventHandler& Glinde_World_EventHandler_create(Glinde::Log& log);
	}

#endif
