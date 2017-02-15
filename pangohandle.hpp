//@	{
//@	 "targets":[{"name":"pangohandle.hpp","type":"include"}]
//@	}

#include "textrenderer.hpp"
#include <pango/pango.h>

PangoContext* pangocontext(PageComposer::Handle<PageComposer::TextRenderer::Context>& ctx)
	{return reinterpret_cast<PangoContext*>(ctx.handle());}

const PangoContext* pangocontext(const PageComposer::Handle<PageComposer::TextRenderer::Context>& ctx)
	{return reinterpret_cast<const PangoContext*>(ctx.handle());}
