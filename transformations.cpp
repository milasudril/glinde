#ifdef __WAND__
target[name[transformations.o] type[object]]
#endif

#include "transformations.h"

using namespace Glinie;

glm::mat3x3 Glinie::basisFromVector(const glm::vec3& n)
	{
	auto a=glm::vec3(n[2],n[0],n[1]);
	auto v=normalize( glm::cross(n,a) );
	auto w=glm::cross(n,v);
	return glm::mat3x3(n,v,w);
	}
