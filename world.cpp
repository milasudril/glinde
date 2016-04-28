#ifdef __WAND__
target[name[world.o] type[object]]
#endif

#include "world.h"
#include "archive.h"
#include "resourceobject.h"
#include "model.h"
#include "debug.h"

#include "facerejectiontree.h"

using namespace Glinda;

World::World(Archive& source):m_textures(source)
	{
//	Spawn player
	m_objects.append(WorldObject{});


	ResourceObject classes{source.fileGet("classes.json")};

	auto i=classes.objectIteratorGet();
	while(!i.endAt())
		{
		auto pair=i.get();
		GLINDA_DEBUG_PRINT("Got key \"%s\"",pair.first);
		auto subobject=pair.second.objectGet("model");

		const char* mesh_path=static_cast<const char*>(subobject);
		GLINDA_DEBUG_PRINT("Got a model object \"%s\"",mesh_path);

		auto ip=m_models.emplace(
			Stringkey(pair.first),Model(m_textures,source.fileGet(mesh_path))
		);

		WorldObject obj;

		obj.modelSet(&ip.first->second);
		m_objects.append(std::move(obj));

		i.next();
		}
	m_tree=new FaceRejectionTree(*mapGet().modelGet(),0);
	n_faces=0;
	}

World::~World()
	{delete m_tree;}

void World::update(uint64_t frame,double delta_t,int64_t wallclock_utc)
	{
	auto ptr=m_objects.begin();
	auto ptr_end=m_objects.end();
	auto dt=static_cast<float>(delta_t);

	while(ptr!=ptr_end)
		{
	//	Integrate position
		if(ptr!=&mapGet())
			{
			auto x=ptr->positionGet();
			auto v=ptr->velocityGet();

			x+=dt * v;


		/*	auto faces=m_tree->facesGet(glm::vec4(x,1.0f),glm::vec3(0.125f,0.125f,0.125f));
			if(faces.length()!=n_faces)
				{
				n_faces=faces.length();
				GLINDA_DEBUG_PRINT("%zu Number of faces: %u",frame,n_faces);
				}*/


		//	TODO: Interaction with other objects?
		//	v+=dt * ptr->accelerationGet();

			ptr->positionSet(x);
			}
		++ptr;
		}
	}
