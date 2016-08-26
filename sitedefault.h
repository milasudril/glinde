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
#include "message.h"

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

			uint32_t itemSpawnSync(const Stringkey& mapspot,const Stringkey& classname);

			uint32_t itemSpawnAsync(uint64_t delay,const Stringkey& mapspot,const Stringkey& classname);

			void spotsVisit(SpotVisitor&& visitor);

			uint32_t playerGet() const noexcept
				{return m_player;}

			SiteDefault& playerSet(uint32_t tid) noexcept
				{
				m_player=tid;
				return *this;
				}

			WorldObject* objectGet(uint32_t tid) noexcept
				{return m_objects.objectGet(tid);}

			const Model& modelGet() const noexcept
				{return *r_model;}

		private:
			const Map* r_map;
			World* r_world;
			EventHandler* r_eh;
			FaceRejectionTree m_tree;
			const Model* r_model;
			ObjectManager m_objects;

			struct ItemSpawnMessage
				{
			/*	Stringkey mapspot;
				Stringkey classname;*/
				};

			class ItemSpawner
				{
				public:
					ItemSpawner(ObjectManager& manager);
					void operator()(uint64_t time,const ItemSpawnMessage& msg);
				private:
					ObjectManager& r_manager;
				};
			Message::Processor<ItemSpawnMessage,ItemSpawner> m_spawner;
			uint32_t m_player;
		};
	}

#endif
