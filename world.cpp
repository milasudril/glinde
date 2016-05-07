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
#include "transformations.h"

using namespace Glinde;

World::World(Archive& source):m_textures(source)
	{
	ResourceObject classes{source.fileGet("classes.json")};

	auto i=classes.objectIteratorGet();
	while(!i.endAt())
		{
		auto pair=i.get();
		GLINDE_DEBUG_PRINT("Got key \"%s\"",pair.first);
		auto subobject=pair.second.objectGet("model");

		const char* mesh_path=static_cast<const char*>(subobject);
		GLINDE_DEBUG_PRINT("Got a model object \"%s\"",mesh_path);

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

glm::vec3 collisionCheck(const FaceRejectionTree& tree,const WorldObject& object
	,const glm::vec3& offset
	,const glm::vec3& v_current)
	{
	auto& frame_current=object.frameCurrentGet();
	auto& bb=frame_current.bounding_box;
	auto v=v_current;

	auto faces=tree.facesFind(glm::vec4(offset,1.0f) + bb.mid()
		,2.0f*bb.size());

	while(faces.first!=faces.second)
		{
		auto& face=*faces.first;
		if(intersect(face - offset,frame_current.meshes))
			{
			auto n=glm::normalize(glm::cross(face[1] - face[0],face[2]-face[0]));
			auto R=basisFromVector(n);
			auto v_normal=glm::transpose(R)*v;
			auto J=object.normalImpulseGet();
			auto m=object.massGet();

			v_normal[0]=std::max(0.0f,glm::dot(J,n)/m);
			v=R*v_normal;
			}
		++faces.first;
		}
	return v;
	}

void World::update(uint64_t frame,double delta_t,int64_t wallclock_utc)
	{
	auto ptr=m_objects.begin();
	auto ptr_end=m_objects.end();
	auto dt=static_cast<float>(delta_t);
	const glm::vec3 g(0.0f,0.0f,-9.81f);
	while(ptr!=ptr_end)
		{
	//	Integrate position
		if(ptr!=&mapGet())
			{
			auto x=ptr->positionGet();
			auto v=ptr->velocityGet();

			auto c=ptr->dampingGet();
			auto F=ptr->forceGet();
			auto m=ptr->massGet();

			auto a=( F - c*glm::vec3(v.x,v.y,0.0f) + m*g )/m;
			v+=dt*a;

			if(ptr->modelGet()!=nullptr && length(v)>0.0f)
				{
				v=collisionCheck(*m_tree,*ptr,x + dt*v,v);
				}

			x+=dt*v;
			ptr->normalImpulseSet(glm::vec3(0.0f,0.0f,0.0f));
			ptr->velocitySet(v);
			ptr->positionSet(x);
			}
		++ptr;
		}
	}
