#ifdef __WAND__
target[name[world.o] type[object]]
#endif

#include "world.h"
#include "archive.h"
#include "resourceobject.h"
#include "debug.h"
#include "facerejectiontree.h"

using namespace Glinda;

World::World(Archive& source):m_textures(source)
	{
//	Spawn player
	m_objects.append(WorldObject{});


	ResourceObject models{source.fileGet("models.json")};

	auto i=models.objectIteratorGet();
	while(!i.atEnd())
		{
		auto pair=i.get();
		GLINDA_DEBUG_PRINT("Got key \"%s\"",pair.first);
		auto subobject=pair.second.objectGet("mesh");

		const char* mesh_path=static_cast<const char*>(subobject);
		GLINDA_DEBUG_PRINT("Got a mesh object \"%s\"",mesh_path);

		auto ip=m_meshes.emplace(Stringkey(pair.first),Mesh(m_textures,source.fileGet(mesh_path)));


		WorldObject obj;
		obj.meshSet(&ip.first->second);
		m_objects.append(std::move(obj));

		i.next();
		}

	m_tree=new FaceRejectionTree(*mapGet().meshGet());
	}

World::~World()
	{}

void World::update(uint64_t frame,double delta_t,int64_t wallclock_utc)
	{
	auto ptr=m_objects.begin();
	auto ptr_end=m_objects.end();
	auto dt=static_cast<float>(delta_t);
	while(ptr!=ptr_end)
		{
	//	Integrate position
		auto x=ptr->positionGet();
		auto v=ptr->velocityGet();

		x+=dt * v;

	//	TODO: Interaction with other objects?
	//	v+=dt * ptr->accelerationGet();

		ptr->positionSet(x);

		++ptr;
		}
	}
