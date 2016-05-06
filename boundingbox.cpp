#ifdef __WAND__
target[name[boundingbox.o] type[object]]
#endif

#include "boundingbox.h"
#include "range.h"

using namespace Glinie;

bool Glinie::insideAny(const Range<const glm::vec3*>& vertices,const BoundingBox& box) noexcept
	{
	auto v=vertices.begin();
	auto end=vertices.end();
	while(v!=end)
		{
		if(inside(glm::vec4(*v,1.0f),box))
			{return 1;}
		++v;
		}
	return 0;
	}

bool Glinie::insideAll(const Range<const glm::vec3*>& vertices,const BoundingBox& box) noexcept
	{
	auto v=vertices.begin();
	auto end=vertices.end();
	while(v!=end)
		{
		if(!inside(glm::vec4(*v,1.0f),box))
			{return 0;}
		++v;
		}
	return 1;
	}

ArrayFixed<Face,12> Glinie::facesGet(const BoundingBox& bb) noexcept
	{
	return ArrayFixed<Face,12>
		{
	//	Side (0)
		 Face
			{
			 glm::vec3(bb.min.x,bb.min.y,bb.min.z)
			,glm::vec3(bb.max.x,bb.min.y,bb.min.z)
			,glm::vec3(bb.max.x,bb.min.y,bb.max.z)
			}
		,Face
			{
			 glm::vec3(bb.max.x,bb.min.y,bb.max.z)
			,glm::vec3(bb.min.x,bb.min.y,bb.max.z)
			,glm::vec3(bb.min.x,bb.min.y,bb.min.z)
			}

	//	Side (1)
		,Face
			{
			 glm::vec3(bb.max.x,bb.min.y,bb.min.z)
			,glm::vec3(bb.max.x,bb.max.y,bb.min.z)
			,glm::vec3(bb.max.x,bb.max.y,bb.max.z)
			}
		,Face
			{
			 glm::vec3(bb.max.x,bb.max.y,bb.max.z)
			,glm::vec3(bb.max.x,bb.min.y,bb.max.z)
			,glm::vec3(bb.max.x,bb.min.y,bb.min.z)
			}

	//	Side (2)
		,Face
			{
			 glm::vec3(bb.max.x,bb.max.y,bb.min.z)
			,glm::vec3(bb.min.x,bb.max.y,bb.min.z)
			,glm::vec3(bb.min.x,bb.max.y,bb.max.z)
			}
		,Face
			{
			 glm::vec3(bb.min.x,bb.max.y,bb.max.z)
			,glm::vec3(bb.max.x,bb.max.y,bb.max.z)
			,glm::vec3(bb.max.x,bb.max.y,bb.min.z)
			}

	//	Side (3)
		,Face
			{
			 glm::vec3(bb.max.x,bb.max.y,bb.min.z)
			,glm::vec3(bb.max.x,bb.min.y,bb.min.z)
			,glm::vec3(bb.max.x,bb.min.y,bb.max.z)
			}
		,Face
			{
			 glm::vec3(bb.max.x,bb.min.y,bb.max.z)
			,glm::vec3(bb.max.x,bb.max.y,bb.max.z)
			,glm::vec3(bb.max.x,bb.max.y,bb.min.z)
			}

	//	Side (4)
		,Face
			{
			 glm::vec3(bb.min.x,bb.min.y,bb.max.z)
			,glm::vec3(bb.max.x,bb.min.y,bb.max.z)
			,glm::vec3(bb.max.x,bb.max.y,bb.max.z)
			}
		,Face
			{
			 glm::vec3(bb.max.x,bb.max.y,bb.max.z)
			,glm::vec3(bb.min.x,bb.max.y,bb.max.z)
			,glm::vec3(bb.min.x,bb.min.y,bb.max.z)
			}

	//	Side (5)
		,Face
			{
			 glm::vec3(bb.max.x,bb.min.y,bb.min.z)
			,glm::vec3(bb.min.x,bb.min.y,bb.min.z)
			,glm::vec3(bb.min.x,bb.max.y,bb.min.z)
			}
		,Face
			{
			 glm::vec3(bb.min.x,bb.max.y,bb.min.z)
			,glm::vec3(bb.max.x,bb.max.y,bb.min.z)
			,glm::vec3(bb.max.x,bb.min.y,bb.min.z)
			}
		};
	}
