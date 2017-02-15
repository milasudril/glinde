//@	{
//@	 "targets":[{"name":"textrenderer.o","type":"object","pkgconfig_libs":["pangocairo"]}]
//@	}

#include "textrenderer.hpp"
#include "rendercontext.hpp"
#include "cairohandle.hpp"
#include <pango/pangocairo.h>

using namespace PageComposer;

TextRenderer::TextRenderer(RenderContext& rc):
	m_handle(pango_cairo_create_context(cairocontext(rc.handle() ) ) )
	{}

TextRenderer::~TextRenderer()
	{g_object_unref(m_handle.handle());}
