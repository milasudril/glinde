//@	{
//@	 "targets":[{"name":"rendercontext.o","type":"object","pkgconfig_libs":["cairo"]}]
//@	}

#include "rendercontext.hpp"
#include "cairohandle.hpp"

using namespace PageComposer;

RenderContext::RenderContext(Surface& surf)
	{}

RenderContext::~RenderContext()
	{cairo_destroy(cairocontext(m_handle));}