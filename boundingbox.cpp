#ifdef __WAND__
target[name[boundingbox.o] type[object]]
#endif

#include "boundingbox.h"
#include "range.h"

using namespace Glinda;

bool Glinda::insideAny(const Range<const glm::vec3*>& vertices,const BoundingBox& box)
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

bool Glinda::insideAll(const Range<const glm::vec3*>& vertices,const BoundingBox& box)
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
