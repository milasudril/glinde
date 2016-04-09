#ifdef __WAND__
target[name[world.h] type[include]]
dependency[world.o]
#endif

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

	class World
		{
		public:
			~World();
			World(Archive& source);

			WorldObject* objectsBegin() noexcept
				{return m_objects.begin();}

			WorldObject* objectsEnd() noexcept
				{return m_objects.end();}

		private:
			std::map<Stringkey, Mesh> m_meshes;
			ArrayDynamic<WorldObject> m_objects;
			TextureManager m_textures;
		};
	}

#endif
