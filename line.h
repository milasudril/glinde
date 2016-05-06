#ifdef __WAND__
target[name[line.h] type[include]]
#endif

#ifndef GLINIE_LINE_H
#define GLINIE_LINE_H

#include <glm/vec4.hpp>

namespace Glinie
	{
	struct Line
		{
		glm::vec4 from;
		glm::vec4 to;
		};
	}

#endif
