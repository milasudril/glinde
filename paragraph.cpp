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

Paragraph& Paragraph::textSet(const char* src)
	{
	pango_layout_set_text(layout(m_layout),src,-1);
	return *this;
	}

Rectangle Paragraph::boundingRectangle() const noexcept
	{
	auto handle=const_cast<PangoLayout*>(layout(m_layout));
	PangoRectangle ink;
	pango_layout_get_pixel_extents(handle,&ink,NULL);
	return Rectangle
		{
		 static_cast<float>(ink.x)
		,static_cast<float>(ink.y)
		,static_cast<float>(ink.width)
		,static_cast<float>(ink.height)
		};
	}
