#ifdef __WAND__
target[name[world.h] type[include]]
dependency[world.o]
#endif

/**\file world.h \brief Defines the World class
 */

#ifndef GLINDA_WORLD_H
#define GLINDA_WORLD_H

#include "arraydynamic.h"
#include "mesh.h"
#include "worldobject.h"
#include "texturemanager.h"
#include "stringkey.h"

namespace Glinda
	{
	class Archive;
	class FaceRejectionTree;

	/**\brief Class representing the current game.
	*/
	class World
		{
		public:
			~World();
			World(Archive& source);

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
				{return m_objects[0];}

			const WorldObject& mapGet() const noexcept
				{return m_objects[1];}

		private:
			std::map<Stringkey, Mesh> m_meshes;
			ArrayDynamic<WorldObject> m_objects;
			TextureManager m_textures;

			FaceRejectionTree* m_tree;
		};
	}

#endif
