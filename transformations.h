//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"transformations.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"transformations.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_TRANSFORMATINOS_H
#define GLINDE_TRANSFORMATINOS_H

#include <glm/glm.hpp>

namespace Glinde
	{
	glm::mat3x3 basisFromVector(const glm::vec3& n);
	}

#endif
