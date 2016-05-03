#ifdef __WAND__
target[name[line.h] type[include]]
#endif

#ifndef GLINDA_LINE_H
#define GLINDA_LINE_H

#include <glm/vec4.hpp>

namespace Glinda
	{
	struct Line
		{
		glm::vec4 from;
		glm::vec4 to;
		};
	}

#endif
