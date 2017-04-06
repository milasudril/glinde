//@	{
//@	 "targets":[{"name":"cairohandles.hpp","type":"include"}]
//@	}

#ifndef PAGECOMPOSER_CAIROHANDLES_HPP
#define PAGECOMPOSER_CAIROHANDLES_HPP

#include "handle.hpp"
#include <cairo.h>

namespace PageComposer
	{
	inline cairo_t* cairocontext(Handle<rc_t>& ctx)
		{return reinterpret_cast<cairo_t*>(ctx.handle());}

	inline const cairo_t* cairocontext(const Handle<rc_t>& ctx)
		{return reinterpret_cast<const cairo_t*>(ctx.handle());}

	inline cairo_surface_t* cairosurface(Handle<surface_t>& ctx)
		{return reinterpret_cast<cairo_surface_t*>(ctx.handle());}

	inline const cairo_surface_t* cairosurface(const Handle<surface_t>& ctx)
		{return reinterpret_cast<const cairo_surface_t*>(ctx.handle());}

	inline cairo_pattern_t* cairopattern(Handle<pattern_t>& pattern)
		{return reinterpret_cast<cairo_pattern_t*>(pattern.handle());}

	inline const cairo_pattern_t* cairopattern(const Handle<pattern_t>& pattern)
		{return reinterpret_cast<const cairo_pattern_t*>(pattern.handle());}
	}

#endif
