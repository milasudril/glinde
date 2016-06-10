#ifdef __WAND__
target[name[worlddefault.h] type[include]]
dependency[worlddefault.o]
#endif

/**\file worlddefault.h \brief Defines the WorldDefault class
 */

#ifndef GLINDE_WORLDDEFAULT_H
#define GLINDE_WORLDDEFAULT_H

#include "resourcemanager.h"
#include "actionprogram.h"
#include "world.h"

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
				r_site=&m_resources.siteGet(key);
				return *this;
				}

		private:
			std::unique_ptr<Filesystem> m_fs;
			ActionProgram m_program;
			EventHandler& r_world_eh;
			ResourceManager m_resources;
			Site* r_site;
		};
	}

#endif
