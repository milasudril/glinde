//@	{
//@	 "targets":[{"name":"surfacehandle.hpp","type":"include"}]
//@	}

#ifndef PAGECOMPOSER_SURFACEHANDLE_HPP
#define PAGECOMPOSER_SURFACEHANDLE_HPP

#include <cairo.h>

inline cairo_surface_t* cairosurface(PageComposer::Handle<PageComposer::Surface::SurfaceType>& ctx)
	{return reinterpret_cast<cairo_surface_t*>(ctx.handle());}

inline const cairo_surface_t* cairocontext(const PageComposer::Handle<PageComposer::Surface::SurfaceType>& ctx)
	{return reinterpret_cast<const cairo_surface_t*>(ctx.handle());}

#endif
