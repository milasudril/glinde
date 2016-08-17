//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"worlddefault.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"worlddefault.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
/**\file worlddefault.h \brief Defines the WorldDefault class
 */

#ifndef GLINDE_WORLDDEFAULT_H
#define GLINDE_WORLDDEFAULT_H

#include "world.h"
#include "resourcemanagerdefault.h"
#include "actionprogram.h"
#include "site.h"
#include "messagequeue.h"

#include <memory>

namespace Glinde
	{
	class Filesystem;
	class Stringkey;

	/**\brief Class representing the current game.
	*/
	class WorldDefault:public World
		{
		public:
			explicit WorldDefault(const char* filename);

			~WorldDefault();

			WorldDefault& siteSet(const Stringkey& key)
				{
				r_site=&m_resources.siteGet(key,*this);
				return *this;
				}

			SiteDefault* siteGet() noexcept
				{return r_site;}

			void siteCreated(Site& site)
				{
				m_world_eh->onSiteCreated(*this,site);
				site.init();
				}

			void siteDestroying(Site& site) noexcept
				{m_world_eh->onSiteDestroy(*this,site);}

			void siteMoved(Site& site) noexcept
				{m_world_eh->onSiteMoved(*this,site);}


		private:
			std::unique_ptr<Filesystem> m_fs;
			ActionProgram m_program;
			std::unique_ptr<EventHandler,EventHandlerDisposer> m_world_eh;
			ResourceManagerDefault m_resources;
			SiteDefault* r_site;

			static decltype(m_world_eh)
			eventHandlerCreate(const ActionProgram& program);
		};
	}

#endif
