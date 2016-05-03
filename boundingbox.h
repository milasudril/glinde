#ifdef __WAND__
target[name[boundingbox.h] type[include]]
dependency[boundingbox.o]
#endif

/**\file boundingbox.h \file Defines the BoundingBox class
 */

#ifndef GLINDA_BOUNDINGBOX_H
#define GLINDA_BOUNDINGBOX_H

#include "face.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace Glinda
	{
	template<class T>
	class Range;

	/**\brief Defines a bounding box
	 *
	 * A BoundingBox is the smallest cuboid that completely encloses a body.
	 *
	*/
	struct BoundingBox
		{
		glm::vec4 min;
		glm::vec4 max;

		/**\brief Computes the extents of the bounding box and returns the result
		 * as a vector.
		*/
		glm::vec3 size() const noexcept
			{return glm::vec3( max - min );}

		glm::vec4 mid() const noexcept
			{return 0.5f*(max + min);}
		};

	/**\brief Tests whether or not `point` is inside the bounding box
	*/
	inline bool inside(const glm::vec4& point,const BoundingBox& bb) noexcept
		{
		auto min=bb.min;
		auto max=bb.max;
		return point.x>=min.x && point.y>=min.y && point.z>=min.z
			&& point.x<max.x && point.y<max.y && point.z<max.z;
		}

	bool insideAny(const Range<const glm::vec3*>& vertices,const BoundingBox& box) noexcept;
	bool insideAll(const Range<const glm::vec3*>& vertices,const BoundingBox& box) noexcept;

	ArrayFixed<Face,12> facesGet(const BoundingBox&) noexcept;
	}

#endif
