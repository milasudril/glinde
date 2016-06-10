#ifdef __WAND__
target[name[line.h] type[include]]
#endif

#ifndef GLINDE_LINE_H
#define GLINDE_LINE_H

#include <glm/vec4.hpp>

namespace Glinde
	{
	struct Line
		{
		glm::vec4 from;
		glm::vec4 to;
		};
	}

#endif