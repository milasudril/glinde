#ifdef __WAND__
target[name[site.o] type[object]]
#endif

#include "site.h"
#include "map.h"
#include "debug.h"
#include "intersections.h"
#include "transformations.h"

using namespace Glinde;

Site::Site(const Map& map):m_tree(map.modelGet(),0)
	{
 	GLINDE_DEBUG_PRINT("Building site from map %s",map.nameGet());
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

void Site::update(uint64_t frame,double delta_t,int64_t wallclock_utc) noexcept
	{
	auto ptr=m_objects.begin();
	auto ptr_end=m_objects.end();
	auto dt=static_cast<float>(delta_t);
	const glm::vec3 g(0.0f,0.0f,0.0f);
	while(ptr!=ptr_end)
		{
		auto& obj=ptr->object();

	//	Integrate position
		if(obj.modelGet()!=r_model)
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
				v=collisionCheck(m_tree,obj,x + dt*v,v);
				}

			x+=dt*v;
			obj.normalImpulseSet(glm::vec3(0.0f,0.0f,0.0f));
			obj.velocitySet(v);
			obj.positionSet(x);
			}
		++ptr;
		}
	}

