#ifdef __WAND__
target[name[world.h] type[include]]
dependency[world.o]
#endif

/**\file world.h \brief Defines the World class
 */

#ifndef GLINDE_WORLD_H
#define GLINDE_WORLD_H

#include "objectmanager.h"
#include "resourcemanager.h"
#include "stringkey.h"
#include "actionprogram.h"

#include <memory>

namespace Glinde
	{
	class Filesystem;
	class FaceRejectionTree;
	class Model;

	/**\brief Class representing the current game.
	*/
	class World
		{
		public:
			~World();

			explicit World(const char* filename);

			const ObjectManager::value_type* objectsBegin() noexcept
				{return m_objects.begin();}

			const ObjectManager::value_type* objectsEnd() noexcept
				{return m_objects.end();}


			/**\brief Updates the current world state
			 *
			 * This function updates the current world state.
			 *
			 * \param frame the number of frames that have elapsed since the
			 *              game started
			 *
			 * \param delta_t the delay between two frames
			 *
			 * \param wallclock_utc the current POSIX time
			 *
			*/
			void update(uint64_t frame,double delta_t,int64_t wallclock_utc);

			WorldObject& playerGet() noexcept
				{return *r_player;}

			const WorldObject& mapGet() const noexcept
				{return *r_map;}

			World& siteSet(const Stringkey& key)
				{
				r_site=&m_resources.siteGet(key);
				return *this;
				}

		private:
			std::unique_ptr<Filesystem> m_fs;
			ActionProgram m_program;
			ResourceManager m_resources;
			ObjectManager m_objects;
			WorldObject* r_player;
			Site* r_site;
			WorldObject* r_map;

			FaceRejectionTree* m_tree;

			uintptr_t nodeid;
		};
	}

#endif
