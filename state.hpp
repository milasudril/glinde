//@	{
//@	 "targets":[{"name":"state.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"state.o","rel":"implementation"}]
//@	}

#ifndef ANGLE_STATE_HPP
#define ANGLE_STATE_HPP

#include "valuetype.hpp"
#include <cassert>

namespace Angle
	{
	template<class U>
	void bindElementIndexData(ValueType&& type,U count)=delete;

	void bindElementIndexData(const ValueType& type,const size_t& count);

	enum class DrawMode:GLenum
		{
		 POINTS=GL_POINTS
		,LINE_STRIP=GL_LINE_STRIP
		,LINE_LOOP=GL_LINE_LOOP
		,LINES=GL_LINES
		,LINE_STRIP_ADJACENCY=GL_LINE_STRIP_ADJACENCY
		,LINES_ADJACENCY=GL_LINES_ADJACENCY
		,TRIANGLE_STRIP=GL_TRIANGLE_STRIP
		,TRIANGLE_FAN=GL_TRIANGLE_FAN
		,TRIANGLES=GL_TRIANGLES
		,TRIANGLE_STRIP_ADJACENCY=GL_TRIANGLE_STRIP_ADJACENCY
		,TRIANGLES_ADJACENCY=GL_TRIANGLES_ADJACENCY
		,PATCHES=GL_PATCHES
		};

	constexpr GLenum native_type(DrawMode mode) noexcept
		{return static_cast<GLenum>(mode);}

	void drawElements(DrawMode mode);

#ifndef NDEBUG
	bool elementsRangeCheck(size_t index_last);
	#define RANGE_CHECK(index_last) assert(elementsRangeCheck(index_last))
#else
	#define RANGE_CHECK(index_last)
#endif

	size_t drawElementsUnchecked(DrawMode mode,size_t index_first,size_t count);

	inline size_t drawElements(DrawMode mode,size_t index_first,size_t count)
		{
		RANGE_CHECK(index_first + count);
		return drawElementsUnchecked(mode,index_first,count);
		}
	}

#endif
