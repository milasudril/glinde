#ifdef __WAND__
target[name[world.o] type[object]]
#endif

#include "world.h"
#include "archive.h"
#include "resourceobject.h"
#include "debug.h"

using namespace Glinda;

World::World()
	{
	}

World::World(Archive&& source)
	{
	ResourceObject models{source.fileGet("models.json")};

	auto i=models.objectIteratorGet();
	while(!i.atEnd())
		{
		auto pair=i.get();
		GLINDA_DEBUG_PRINT("Got key \"%s\"",pair.first);
		auto subobject=pair.second.objectGet("mesh");

		const char* mesh_path=static_cast<const char*>(subobject);
		GLINDA_DEBUG_PRINT("Got a mesh object \"%s\"",mesh_path);

		Mesh mesh(source.fileGet(mesh_path));
		i.next();
		}
	}

World::~World()
	{}
