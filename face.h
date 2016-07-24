//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"face.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"face.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}

/**\file face.h \brief Defines a Face
 */

#ifndef GLINDE_FACE_H
#define GLINDE_FACE_H

#include "arrayfixed.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace Glinde
	{
	/**\brief A lose triangle that is part of a Mesh.
	 *
	 * A Face is a loose triangle that is part of a Mesh. A Face can be used as
	 * an ArrayFixed<glm::vec3,3>.
	 *
	*/
	class Face:public ArrayFixed<glm::vec3,3>
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
	bool intersect(const Face& a,const Range<const Face>& faces) noexcept;


	/**\brief Creates a translated copy of <var>face</var>
	 *
	 * This function creates a translated copy of <var>face</var>
	 *
	 * \param face The face to translate
	 * \param x The offset vector used for the translation transformation.The
	 * offset is added to each of the face vertices.
	 *
	 */
	inline Face operator+(const Face& face,const glm::vec3& x) noexcept
		{
		auto ret=face;
		for(size_t k=0;k<face.length();++k)
			{ret[k]+=x;}
		return ret;
		}

	/**\brief Creates a translated copy of <var>face</var>
	 *
	 * This function creates a translated copy of <var>face</var>
	 *
	 * \param face The face to translate
	 * \param x The offset vector used for the translation transformation. The
	 * offset is subtracted from each of the face vertices.
	 *
	 */
	inline Face operator-(const Face& face,const glm::vec3& x) noexcept
		{
		auto ret=face;
		for(size_t k=0;k<face.length();++k)
			{ret[k]-=x;}
		return ret;
		}

	inline Face operator*(const glm::mat4& matrix,const Face& face)
		{
		auto ret=face;
		for(size_t k=0;k<face.length();++k)
			{
			ret[k]=glm::vec3(matrix*glm::vec4(ret[k],1.0f));
			}
		return ret;
		}
	}
#endif
