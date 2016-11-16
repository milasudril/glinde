//@	{
//@	 "targets":
//@		[{
//@		 "name":"world.so","type":"lib_dynamic"
//@		,"dependencies":[{"ref":"maikeconfig.json","rel":"generated"}]
//@		}]
//@	,"author":["Torbjörn Rathsman"]
//@	,"year":2016
//@	,"license":"Public domain"
//@	,"description":"Sample action program main file"
//@	}

#include "map00/eventhandler.h"
#include "common/syslog.h"
#include <glinde/world.h>
#include <glinde/stringkey.h>
#include <glinde/mapfixed.h>
#include <glinde/log.h>
#include <glinde/variant.h>

using namespace Glinde;

class Init:public World::EventHandler
	{
	public:
		Init()
			{
			g_syslog->write(Log::MessageType::INFORMATION
				,"Welcome to Glinde test world",{});
			m_site_handlers.get<Stringkey("map00")>()=&m_map00;
			}

		void onLoaded(World& world)
			{
			g_syslog->write(Log::MessageType::INFORMATION
				,"Host claims that the world is now loaded. Going to map00.",{});
			world.siteSet(Stringkey("map00"));
			}

		void onUnload(World& world) noexcept
			{
			g_syslog->write(Log::MessageType::INFORMATION
				,"Leaving Glinde test world",{});
			}

		void destroy() noexcept
			{
			g_syslog->write(Log::MessageType::INFORMATION
				,"World is being destroyed",{});
			delete this;
			}


		void onSiteCreated(World& world,Site& site)
			{
			g_syslog->write(Log::MessageType::INFORMATION
				,"Attaching event handler for site #0;",{&site});
			site.eventHandlerSet(*m_site_handlers[site.idGet()]);
			}

		void onSiteDestroy(World& world,Site& site) noexcept
			{
			g_syslog->write(Log::MessageType::INFORMATION
				,"Site #0; is being destroyed",{&site});
			}

		void onSiteMoved(World& world,Site& site) noexcept
			{
			}

	private:
		MapFixed<Stringkey::HashValue,Site::EventHandler*
			,Stringkey("map00")> m_site_handlers;
		Map00::EventHandler m_map00;
	};

World::EventHandler& Glinde_World_EventHandler_create(Log& log)
	{
	g_syslog=&log;
	return *(new Init);
	}
