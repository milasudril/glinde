//@	{
//@	 "targets":[{"name":"pangohandles.hpp","type":"include"}]
//@	}

#ifndef PAGECOMPOSER_PANGOHANDLES_HPP
#define PAGECOMPOSER_PANGOHANDLES_HPP

#include "handle.hpp"
#include <pango/pango.h>

namespace PageComposer
	{
	inline PangoContext* pangocontext(Handle<tr_t>& ctx)
		{return reinterpret_cast<PangoContext*>(ctx.handle());}

	inline const PangoContext* pangocontext(const Handle<tr_t>& ctx)
		{return reinterpret_cast<const PangoContext*>(ctx.handle());}


	inline static const PangoFontDescription* font(const Handle<font_t>& f)
		{return reinterpret_cast<const PangoFontDescription*>( f.handle() );}

	inline static PangoFontDescription* font(Handle<font_t>& f)
		{return reinterpret_cast<PangoFontDescription*>( f.handle() );}


	inline static const PangoLayout* layout(const Handle<layout_t>& f)
		{return reinterpret_cast<const PangoLayout*>( f.handle() );}

	inline static PangoLayout* layout(Handle<layout_t>& f)
		{return reinterpret_cast<PangoLayout*>(f.handle());}
	}

#endif
