//@	{
//@	 "targets":[{"name":"world.so","type":"plugin"}]
//@	,"author":["Torbjörn Rathsman"]
//@	,"year":2016
//@	,"license":"Public domain"
//@	,"description":"Sample action program main file"
//@	}

#include "map00/eventhandler.h"
#include <glinde/world.h>
#include <glinde/stringkey.h>
#include <glinde/mapfixed.h>

#include <cstdio>

using namespace Glinde;

class Init:public World::EventHandler
	{
	public:
		Init()
			{
			m_site_handlers.get<Stringkey("map00")>()=&m_map00;
			}

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
			site.eventHandlerSet(*m_site_handlers[site.idGet()]);
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

	private:
		MapFixed<Stringkey::HashValue,Site::EventHandler*
			,Stringkey("map00")> m_site_handlers;
		Map00::EventHandler m_map00;
	};

World::EventHandler& Glinde_World_EventHandler_create()
	{
	return *(new Init);
	}
