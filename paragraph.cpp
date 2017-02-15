//@	{
//@	 "targets":
//@		[
//@			{"name":"paragraph.o","type":"object","pkgconfig_libs":["pango"] }
//@		]
//@	}

#include "paragraph.hpp"
#include "parastyle.hpp"
#include "textstyle.hpp"
#include "textrenderer.hpp"
#include "pangohandles.hpp"

using namespace PageComposer;

static const ParaStyle s_para_default;
static const TextStyle s_text_default;

Paragraph::Paragraph(TextRenderer& tr):m_font(pango_font_description_new())
	,m_layout(pango_layout_new( pangocontext( tr.handle() ) ) )
	{style(s_para_default).style(s_text_default);}

Paragraph::~Paragraph()
	{
	g_object_unref(m_layout.handle());
	pango_font_description_free(font(m_font));
	}
