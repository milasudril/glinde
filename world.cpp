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
#include "blob.h"
#include "sdkresource.h"

#include "errormessage.h"
#include "engine.h"
GLINDE_BLOB(engine_class,"engine.h");

using namespace Glinde;

static const SdkResource SDK[]
	{
		{"engine.h",Range<const uint8_t>(engine_class_begin,engine_class_end)}
	};

World::World(const char* filename):m_fs(new Archive(filename))
	,m_program(*m_fs.get(),"world.so",Range<const SdkResource>(SDK,1))
	,m_resources(*m_fs.get())
	,r_player(nullptr)
	,r_site(nullptr)
	,r_map(nullptr),m_tree(nullptr)
	{
	m_program.entryPointGet<Engine::EventHandler* (*)()>("eventHandlerCreate")();
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
	const glm::vec3 g(0.0f,0.0f,0.0f);
	while(ptr!=ptr_end)
		{
		auto& obj=ptr->object();

	//	Integrate position
		if(&obj!=&mapGet())
			{
			auto x=obj.positionGet();
			auto v=obj.velocityGet();

			auto c=obj.dampingGet();
			auto F=obj.forceGet();
			auto m=obj.massGet();

			auto a=( F - c*glm::vec3(v.x,v.y,0.0f) + m*g )/m;
			v+=dt*a;

			if(obj.modelGet()!=nullptr && length(v)>0.0f)
				{
			//	v=collisionCheck(*m_tree,*ptr,x + dt*v,v);
				}

			x+=dt*v;
			obj.normalImpulseSet(glm::vec3(0.0f,0.0f,0.0f));
			obj.velocitySet(v);
			obj.positionSet(x);
			}
		++ptr;
		}
	}
