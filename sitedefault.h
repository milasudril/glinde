//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"sitedefault.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"sitedefault.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_SITEDEFAULT_H
#define GLINDE_SITEDEFAULT_H

#include "site.h"
#include "map.h"
#include "facerejectiontree.h"
#include "objectmanager.h"

namespace Glinde
	{
	class Map;
	class Model;
	class World;

	class SiteDefault:public Site
		{
		public:
			SiteDefault(Map&& map)=delete;
			explicit SiteDefault(const Map& map,World& world_notify);
			SiteDefault(SiteDefault&& obj) noexcept;
			SiteDefault& operator=(SiteDefault&& obj) noexcept;
			SiteDefault(const SiteDefault&)=delete;
			SiteDefault& operator=(const SiteDefault&)=delete;

			~SiteDefault();

			Range<ObjectManager::value_type> objectsGet() noexcept
				{return m_objects.objectsGet();}

			Range<const ObjectManager::value_type> objectsGet() const noexcept
				{return m_objects.objectsGet();}

			Site& eventHandlerSet(EventHandler& eh) noexcept
				{
				r_eh=&eh;
				return *this;
				}

			Site& init() noexcept
				{
				if(r_eh!=nullptr)
					{r_eh->onInit(*this);}
				return *this;
				}

			/**\brief Updates the current state
			 *
			 * This function updates the current state.
			 *
			 * \param frame the number of frames that have elapsed since the
			 *              game started
			 *
			 * \param delta_t the delay between two frames
			 *
			 * \param wallclock_utc the current POSIX time
			 *
			*/
			void update(uint64_t frame,double delta_t,int64_t wallclock_utc) noexcept;

			const Stringkey& idGet() const noexcept
				{return r_map->idGet();}

			uint32_t itemSpawn(const Stringkey& mapspot,const Stringkey& classname) noexcept;

			void spotsVisit(SpotVisitor&& visitor);

		private:
			const Map* r_map;
			World* r_world;
			EventHandler* r_eh;
			FaceRejectionTree m_tree;
			const Model* r_model;
			ObjectManager m_objects;
		};
	}

#endif
