//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"transformations.o",
//@	            "type":"object"
//@	        }
//@	    ]
//@	}
#include "transformations.h"

using namespace Glinde;

glm::mat3x3 Glinde::basisFromVector(const glm::vec3& n)
	{
	auto a=glm::vec3(n[2],n[0],n[1]);
	auto v=normalize( glm::cross(n,a) );
	auto w=glm::cross(n,v);
	return glm::mat3x3(n,v,w);
	}
