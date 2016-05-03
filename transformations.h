#ifdef __WAND__
target[name[transformations.h] type[include]]
dependency[transformations.o]
#endif

#ifndef GLINDA_TRANSFORMATINOS_H
#define GLINDA_TRANSFORMATINOS_H

#include <glm/glm.hpp>

namespace Glinda
	{
	glm::mat3x3 basisFromVector(const glm::vec3& n);
	}

#endif
