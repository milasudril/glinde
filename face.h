#ifdef __WAND__
target[name[face.h] type[include]]
dependency[face.o]
#endif

#ifndef GLINIE_FACE_H
#define GLINIE_FACE_H

#include "arrayfixed.h"
#include <glm/vec3.hpp>

namespace Glinie
	{
	struct Face:public ArrayFixed<glm::vec3,3>
		{
		using ArrayFixed<glm::vec3,3>::ArrayFixed;
		};

	bool intersect(const Face& a,const Face& b) noexcept;
	bool intersect(const Face& a,const Range<const Face*>& faces) noexcept;

	inline Face operator+(const Face& face,const glm::vec3& x) noexcept
		{
		auto ret=face;
		for(size_t k=0;k<face.length();++k)
			{ret[k]+=x;}
		return ret;
		}

	inline Face operator-(const Face& face,const glm::vec3& x) noexcept
		{
		auto ret=face;
		for(size_t k=0;k<face.length();++k)
			{ret[k]-=x;}
		return ret;
		}
	}
#endif
