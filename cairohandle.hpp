//@	{
//@	 "targets":[{"name":"cairohandle.hpp","type":"include"}]
//@	}

#ifndef PAGECOMPOSER_CAIROHANDLE_HPP
#define PAGECOMPOSER_CAIROHANDLE_HPP

#include <cairo.h>

inline cairo_t* cairocontext(PageComposer::Handle<PageComposer::RenderContext::Context>& ctx)
	{return reinterpret_cast<cairo_t*>(ctx.handle());}

inline const cairo_t* cairocontext(const PageComposer::Handle<PageComposer::RenderContext::Context>& ctx)
	{return reinterpret_cast<const cairo_t*>(ctx.handle());}

#endif
