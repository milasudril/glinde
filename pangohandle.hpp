//@	{
//@	 "targets":[{"name":"pangohandle.hpp","type":"include"}]
//@	}

#include "textrenderer.hpp"
#include <pango/pango.h>

using namespace PageComposer;

PangoContext* pangocontext(Handle<TextRenderer::Context>& ctx)
	{return reinterpret_cast<PangoContext*>(ctx.handle());}

const PangoContext* pangocontext(const Handle<TextRenderer::Context>& ctx)
	{return reinterpret_cast<const PangoContext*>(ctx.handle());}
