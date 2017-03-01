//@	{
//@	 "targets":[{"name":"rendercontext.o","type":"object","pkgconfig_libs":["cairo"]}]
//@	}

#include "rendercontext.hpp"
#include "surface.hpp"
#include "cairohandles.hpp"
#include "exceptionhandler.hpp"

using namespace PageComposer;

RenderContext::RenderContext(Surface& surf):
	 m_handle(cairo_create(cairosurface(surf.handle() ) ))
	,r_surface(&surf)
	{
	auto status=cairo_status(cairocontext(m_handle));
	if(status!=CAIRO_STATUS_SUCCESS)
		{exceptionRaise(Error("Failed to create a Cairo render context: ",cairo_status_to_string(status)));}
	}

RenderContext::~RenderContext()
	{cairo_destroy(cairocontext(m_handle));}