//@	{
//@	 "targets":[{"name":"pattern.o","type":"object","pkgconfig_libs":["cairo"]}]
//@	}

#include "pattern.hpp"
#include "surface.hpp"
#include "cairohandles.hpp"

using namespace PageComposer;

Pattern::Pattern(RenderContext& rc,const Surface& surface):PageObjectBase(rc),
m_handle(cairo_pattern_create_for_surface(const_cast<cairo_surface_t*>(cairosurface(surface.handle()))) )
	{
	}

Pattern::~Pattern()
	{
	cairo_pattern_destroy(cairopattern(m_handle));
	}
