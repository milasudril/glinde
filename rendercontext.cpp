//@	{
//@	 "targets":[{"name":"rendercontext.o","type":"object","pkgconfig_libs":["cairo"]}]
//@	}

#include "rendercontext.hpp"
#include "surface.hpp"
#include "cairohandles.hpp"

using namespace PageComposer;

RenderContext::RenderContext(Surface& surf):
	 m_handle(cairo_create(cairosurface(surf.handle() ) ))
	,r_surface(&surf)
	{}

RenderContext::~RenderContext()
	{cairo_destroy(cairocontext(m_handle));}