#ifdef __WAND__
target[name[world.o] type[object]]
#endif

#include "world.h"
#include "archive.h"
#include "resourceobject.h"
#include "model.h"
#include "debug.h"
#include "logwriter.h"
#include "facerejectiontree.h"
#include "intersections.h"

using namespace Glinda;

World::World(Archive& source):m_textures(source)
	{
	ResourceObject classes{source.fileGet("classes.json")};

	auto i=classes.objectIteratorGet();
	while(!i.endAt())
		{
		auto pair=i.get();
		GLINDA_DEBUG_PRINT("Got key \"%s\"",pair.first);
		auto subobject=pair.second.objectGet("model");

		const char* mesh_path=static_cast<const char*>(subobject);
		GLINDA_DEBUG_PRINT("Got a model object \"%s\"",mesh_path);

		auto key=Stringkey(pair.first);

		auto ip=m_models.emplace(key,Model(m_textures,source.fileGet(mesh_path)));

		WorldObject obj;

		obj.modelSet(&ip.first->second);
		m_objects.append(std::move(obj));

		if(key==Stringkey("player"))
			{
			r_player=m_objects.end() - 1;
			}

		if(key==Stringkey("world"))
			{
			r_map=m_objects.end() - 1;
			}

		i.next();
		}
	logWrite(LogMessageType::INFORMATION,"Building face rejection tree");
	m_tree=new FaceRejectionTree(*mapGet().modelGet(),0);
	nodeid=0;
	}

World::~World()
	{delete m_tree;}

static Twins<FaceRejectionTree::FaceIterator>
collisionCheck(const FaceRejectionTree& tree,const WorldObject& object
	,const glm::vec3& offset)
	{
	auto& frame_current=object.frameCurrentGet();
	auto& bb=frame_current.bounding_box;


	auto faces=tree.facesFind(glm::vec4(offset,1.0f) + bb.mid()
		,2.0f*bb.size());
	auto k=0;
	while(faces.first!=faces.second)
		{
		if(intersect(*faces.first - offset,frame_current.meshes))
			{
			return faces;
			}
		++k;
		++faces.first;
		}
	return faces;
	}

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


		//	TODO: Interaction with other objects?
		//	v+=dt * ptr->accelerationGet();

			if(ptr->modelGet()!=nullptr && length(v)>0.0f)
				{
				auto x_temp=x + dt*v;
				auto check=collisionCheck(*m_tree,*ptr,x_temp);
				if(check.first==check.second)
					{x=x_temp;}
				else
					{
					auto& face=*check.first;
					auto n=glm::normalize(glm::cross(face[1] - face[0],face[2]-face[0]));
					GLINDA_DEBUG_PRINT("%zu %.8g %.8g %.8g",n[0],n[1],n[2],frame);
					v=0.125f*(v - 2.0f*(glm::dot(v,n)*n));
					x_temp=x + dt*v;
					auto check=collisionCheck(*m_tree,*ptr,x_temp);
					if(check.first==check.second)
						{
						x=x_temp;
						}
					}

				}
		//	v+=dt*glm::vec3(0.0f,0.0f,-9.81f);
			ptr->velocitySet(v);
			ptr->positionSet(x);
			}
		++ptr;
		}
	}
