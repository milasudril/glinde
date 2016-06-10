#ifdef __WAND__
target[name[site.h] type[include]]
dependency[site.o]
#endif

#ifndef GLINDE_SITE_H
#define GLINDE_SITE_H

#include "facerejectiontree.h"
#include "objectmanager.h"

namespace Glinde
	{
	class Map;
	class Model;

	class Site
		{
		public:
			explicit Site(const Map& map);
			Range<ObjectManager::value_type> objectsGet() noexcept
				{return m_objects.objectsGet();}

			Range<const ObjectManager::value_type> objectsGet() const noexcept
				{return m_objects.objectsGet();}


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

		private:
			FaceRejectionTree m_tree;
			const Model* r_model;
			ObjectManager m_objects;
		};
	}

#endif
