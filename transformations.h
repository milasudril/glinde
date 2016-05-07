#ifdef __WAND__
target[name[transformations.h] type[include]]
dependency[transformations.o]
#endif

#ifndef GLINDE_TRANSFORMATINOS_H
#define GLINDE_TRANSFORMATINOS_H

#include <glm/glm.hpp>

namespace Glinde
	{
	glm::mat3x3 basisFromVector(const glm::vec3& n);
	}

#endif
