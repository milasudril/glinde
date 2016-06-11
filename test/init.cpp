//@	{
//@	 "targets":[{"name":"world.so","type":"plugin"}]
//@	,"author":["Torbjörn Rathsman"]
//@	,"year":2016
//@	,"license":"Public domain"
//@	,"description":"Sample action program main file"
//@	}

#include "common/hello.h"
#include <glinde/world.h>
#include <glinde/stringkey.h>
#include <cstdio>

using namespace Glinde;

class Init:public World::EventHandler
	{
	public:
		void onLoaded(World& world)
			{
			world.siteSet(Stringkey("map00"));
			}

		void onUnload(World& world) noexcept
			{
			fprintf(stderr,"Good bye\n");
			}

		void destroy() noexcept
			{
			fprintf(stderr,"I am going to die now\n");
			delete this;
			}


		void onSiteCreated(World& world,Site& site)
			{
#if 0
			site->eventHandlerSet(m_handlers[site.idGet()]);
#endif
			fprintf(stderr,"Created site %p\n",&site);
			}

		void onSiteDestroy(World& world,Site& site) noexcept
			{
			fprintf(stderr,"Site %p is being destroyed\n",&site);
			}

		void onSiteMoved(World& world,Site& site) noexcept
			{
			fprintf(stderr,"Site moved to %p\n",&site);
			}
	};

World::EventHandler& Glinde_World_EventHandler_create()
	{
	return *(new Init);
	}
