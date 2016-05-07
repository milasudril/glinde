#ifdef __WAND__
target[name[boundingbox.h] type[include]]
dependency[boundingbox.o]
#endif

/**\file boundingbox.h \brief Defines the BoundingBox class
 */

#ifndef GLINIE_BOUNDINGBOX_H
#define GLINIE_BOUNDINGBOX_H

#include "face.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace Glinie
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
		 *
		 * This function computes the extents of the bounding box.
		 *
		*/
		glm::vec3 size() const noexcept
			{return glm::vec3( max - min );}

		/**\brief Computes the midpoint of the bounding box.
		 *
		 * This function computes the midpoint of the bounding box.
		*/
		glm::vec4 mid() const noexcept
			{return 0.5f*(max + min);}
		};

	/**\brief Tests whether or not <var>point</var> is inside the bounding box
	 *
	 * This function tests whether or not <var>point</var> is inside <var>bb</var>.
	 * box.
	 *
	 * \return A non-zero value if and only <var>point</var> is inside <var>bb</var>.
	 *
	*/
	inline bool inside(const glm::vec4& point,const BoundingBox& bb) noexcept
		{
		auto min=bb.min;
		auto max=bb.max;
		return point.x>=min.x && point.y>=min.y && point.z>=min.z
			&& point.x<max.x && point.y<max.y && point.z<max.z;
		}

	/**\brief Tests whether or not <em>any</em> of the given vertices lay inside
	 * the bounding box <var>box</var>.
	 *
	 * This function tests whether or not <em>any</em> of the given vertices lay inside
	 * the bounding box <var>box</var>.
	 *
	 * \return A non-zero value non-zero if and only if any of the vertices is
	 * inside <var>box</var>.
	 *
	*/
	bool insideAny(const Range<const glm::vec3*>& vertices,const BoundingBox& box) noexcept;

	/**\brief Tests whether or not <em>all</em> of the given vertices lay inside
	 * the bounding box <var>box</var>.
	 *
	 * This function tests whether or not <em>all</em> of the given vertices lay inside
	 * the bounding box <var>box</var>.
	 *
	 * \return A non-zero value if and only if all vertices is inside <var>box</var>.
	*/
	bool insideAll(const Range<const glm::vec3*>& vertices,const BoundingBox& box) noexcept;

	/**\brief Generates a fixed array containing all 12 faces enclosing <var>box</var>.
	 *
	 * This function generates a fixed array containing all 12 faces enclosing <var>box</var>.
	*/
	ArrayFixed<Face,12> facesGet(const BoundingBox& box) noexcept;
	}

#endif
