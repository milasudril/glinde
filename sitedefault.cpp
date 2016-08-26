//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"sitedefault.o",
//@	            "type":"object"
//@	        }
//@	    ]
//@	}
#include "sitedefault.h"
#include "world.h"
#include "debug.h"
#include "intersections.h"
#include "transformations.h"
#include "variant.h"

using namespace Glinde;

SiteDefault::SiteDefault(const Map& map,World& world_notify):r_map(&map)
	,r_world(&world_notify),r_eh(nullptr),m_tree(map.modelGet(),0)
	,m_spawner(m_objects),m_player(0)
	{
 	GLINDE_DEBUG_PRINT("Building site from map #0;",{map.nameGet()});
	r_model=&map.modelGet();
	auto items=map.itemsGet();
	auto ptr=items.begin();
	auto ptr_end=items.end();
	while(ptr!=ptr_end)
		{
		if(ptr->classGet()!=nullptr)
			{m_objects.insert(WorldObject(*ptr));}
		++ptr;
		}
	world_notify.siteCreated(*this);
	}

SiteDefault::SiteDefault(SiteDefault&& obj) noexcept:
	 r_map(obj.r_map),r_world(obj.r_world),r_eh(obj.r_eh)
	,m_tree(std::move(obj.m_tree))
	,r_model(obj.r_model),m_objects(std::move(obj.m_objects))
	,m_spawner(m_objects),m_player(obj.m_player)
	{
	obj.r_world=nullptr;
	r_world->siteMoved(*this);
	}

SiteDefault& SiteDefault::operator=(SiteDefault&& obj) noexcept
	{
	std::swap(r_map,obj.r_map);
	std::swap(r_world,obj.r_world);
	std::swap(r_eh,obj.r_eh);
	std::swap(m_tree,obj.m_tree);
	std::swap(r_model,obj.r_model);
	std::swap(m_objects,obj.m_objects);
	std::swap(m_player,obj.m_player);
	r_world->siteMoved(*this);
	return *this;
	}

SiteDefault::~SiteDefault()
	{
	if(r_world!=nullptr)
		{r_world->siteDestroying(*this);}
	}

void SiteDefault::spotsVisit(SpotVisitor&& visitor)
	{
	}


SiteDefault::ItemSpawner::ItemSpawner(ObjectManager& manager):r_manager(manager)
	{}

void SiteDefault::ItemSpawner::operator()(uint64_t time,const ItemSpawnMessage& msg)
	{

	}
	

uint32_t SiteDefault::itemSpawnSync(const Stringkey& mapspot,const Stringkey& classname)
	{
	auto item=*r_map->itemFind(mapspot);
	GLINDE_DEBUG_PRINT("Spawning an item at #0; #1; #2; [m_spawner #3;]",&item
		,static_cast<Stringkey::HashValue>(mapspot)
		,static_cast<Stringkey::HashValue>(classname),&m_spawner);
	item.classSet(r_world->resourcesGet(),classname);
	auto id=m_objects.idGet();
	m_objects.insert(WorldObject(item),id);
	return id;
	}

uint32_t SiteDefault::itemSpawnAsync(uint64_t delay,const Stringkey& mapspot,const Stringkey& classname)
	{
	auto item=*r_map->itemFind(mapspot);
	GLINDE_DEBUG_PRINT("Spawning an item at #0; #1; #2; [m_spawner #3;]",&item
		,static_cast<Stringkey::HashValue>(mapspot)
		,static_cast<Stringkey::HashValue>(classname),&m_spawner);

	r_world->messagePost(Message(Message::Time(0)
		,m_spawner,ItemSpawnMessage{}));
	return -1;
	}

static
glm::vec3 collisionCheck(const FaceRejectionTree& tree,const WorldObject& object
	,const glm::vec3& offset
	,const glm::vec3& v_current) noexcept
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

void SiteDefault::update(uint64_t frame,double delta_t,int64_t wallclock_utc) noexcept
	{
	auto ptr=m_objects.begin();
	auto ptr_end=m_objects.end();
	auto dt=static_cast<float>(delta_t);
	const glm::vec3 g(0.0f,0.0f,0.0f);
	while(ptr!=ptr_end)
		{
		auto& obj=ptr->object();

	//	Integrate position
		auto x=obj.positionGet();
		auto v=obj.velocityGet();

		auto c=obj.dampingGet();
		auto F=obj.forceGet();
		auto m=obj.massGet();

		auto a=( F - c*glm::vec3(v.x,v.y,0.0f) + m*g )/m;
		v+=dt*a;

		if(obj.modelGet()!=nullptr && length(v)>0.0f)
			{
			v=collisionCheck(m_tree,obj,x + dt*v,v);
			}

		x+=dt*v;
		obj.normalImpulseSet(glm::vec3(0.0f,0.0f,0.0f));
		obj.velocitySet(v);
		obj.positionSet(x);

		++ptr;
		}
	}

