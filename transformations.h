#ifdef __WAND__
target[name[transformations.h] type[include]]
dependency[transformations.o]
#endif

#ifndef GLINIE_TRANSFORMATINOS_H
#define GLINIE_TRANSFORMATINOS_H

#include <glm/glm.hpp>

namespace Glinie
	{
	glm::mat3x3 basisFromVector(const glm::vec3& n);
	}

#endif
