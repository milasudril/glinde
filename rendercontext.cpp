//@	{
//@	 "targets":[{"name":"rendercontext.o","type":"object","pkgconfig_libs":["cairo"]}]
//@	}

#include "rendercontext.hpp"
#include "surface.hpp"
#include "surfacehandle.hpp"
#include "cairohandle.hpp"

using namespace PageComposer;

RenderContext::RenderContext(Surface& surf):
	m_handle(cairo_create(cairosurface(surf.handle() ) ))
	{}

RenderContext::~RenderContext()
	{cairo_destroy(cairocontext(m_handle));}