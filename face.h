#ifdef __WAND__
target[name[face.h] type[include]]
dependency[face.o]
#endif


/**\file face.h \brief Defines a Face
 */

#ifndef GLINDE_FACE_H
#define GLINDE_FACE_H

#include "arrayfixed.h"
#include <glm/vec3.hpp>

namespace Glinde
	{
	/**\brief A lose triangle that is part of a Mesh.
	 *
	 * A Face is a lose triangle that is part of a Mesh. A Face can be used as
	 * an ArrayFixed<glm::vec3,3>.
	 *
	*/
	struct Face:public ArrayFixed<glm::vec3,3>
		{
		using ArrayFixed<glm::vec3,3>::ArrayFixed;
		};

	/**\brief Tests whether or not <var>a</var> intersects <var>b</var>.
	 *
	 * This function tests whether or not <var>a</var> intersects <var>b</var>.
	 * For intersection to occur, the planes spanned by <var>a</var> and <var>b</var>
	 * must not be coplanar.
	 *
	 * \return A non-zero value if and only if <var>a</var> intersects <var>b</var>
	 *
	 */
	bool intersect(const Face& a,const Face& b) noexcept;

	/**\brief Tests whether or not <var>a</var> intersects any of <var>faces</var>.
	 *
	 * This function tests whether or not <var>a</var> intersects <var>b</var>.
	 * This function is a wrapper for intersect(const Face&,const Face&).
	 *
	 * \return A non-zero value if and only if <var>a</var> intersects any
	 * of <var>faces</var>
	 * \see intersect(const Face&,const Face&)
	 *
	 */
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
