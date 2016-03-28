#ifdef __WAND__
target[name[world.h] type[include]]
dependency[world.o]
#endif

#ifndef GLINDA_WORLD_H
#define GLINDA_WORLD_H

#include "arraysimple.h"
#include "stringkey.h"
#include "mesh.h"
#include "worldobject.h"
#include <map>

namespace Glinda
	{
	class Archive;

	class World
		{
		public:
			World();
			~World();
			World(Archive&& source);

		private:
			std::map<Stringkey, Mesh> m_meshes;
			ArraySimple<WorldObject> m_objects;
		};
	}

#endif
