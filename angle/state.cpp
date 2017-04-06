//@	{
//@	 "targets":[{"name":"state.o","type":"object","pkgconfig_libs":["glew"]}]
//@	}

#include "state.hpp"

using namespace Angle;

static const ValueType* s_element_index_type;
static const size_t* s_element_index_count;

#ifndef NDEBUG
bool Angle::elementsRangeCheck(size_t index_last)
	{
	return index_last<=*s_element_index_count;
	}
#endif

void Angle::bindElementIndexData(const ValueType& type,const size_t& count)
	{
	s_element_index_type=&type;
	s_element_index_count=&count;
	}

size_t Angle::drawElementsUnchecked(DrawMode mode,size_t index_first,size_t count)
	{
	glDrawElements(native_type(mode),static_cast<GLsizei>(count),native_type(*s_element_index_type)
		,reinterpret_cast<void*>(index_first*size(*s_element_index_type)));
	return count;
	}

void Angle::drawElements(DrawMode mode)
	{
	drawElementsUnchecked(mode,0,*s_element_index_count);
	}
