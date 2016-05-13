#ifdef __WAND__
target[name[world.h] type[include]]
dependency[world.o]
#endif

/**\file world.h \brief Defines the World class
 */

#ifndef GLINDE_WORLD_H
#define GLINDE_WORLD_H

#include "arraydynamic.h"
#include "worldobject.h"
#include "resourcemanager.h"
#include "stringkey.h"

namespace Glinde
	{
	class Archive;
	class FaceRejectionTree;
	class Model;

	/**\brief Class representing the current game.
	*/
	class World
		{
		public:
			~World();
			explicit World(Archive& source);

			const WorldObject* objectsBegin() noexcept
				{return m_objects.begin();}

			const WorldObject* objectsEnd() noexcept
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

		private:
			std::map<Stringkey, Model> m_models;
			ArrayDynamic<WorldObject> m_objects;
			WorldObject* r_player;
			WorldObject* r_map;
			ResourceManager m_resources;

			FaceRejectionTree* m_tree;

			uintptr_t nodeid;
		};
	}

#endif
