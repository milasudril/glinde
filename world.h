#ifdef __WAND__
target[name[world.h] type[include]]
#endif

#ifndef GLINDA_WORLD_H
#define GLINDA_WORLD_H

#include "arraysimple.h"

namespace Glinda
	{
	class World
		{
		public:

		private:
			ArraySimple<float> vertices;
			ArraySimple<uint32_t> faces;
		};
	}

#endif
