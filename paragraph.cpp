//@	{
//@	 "targets":
//@		[
//@			{"name":"paragraph.o","type":"object","pkgconfig_libs":["pango"] }
//@		]
//@	}

#include "paragraph.hpp"
#include "parastyle.hpp"
#include "textstyle.hpp"
#include "pangohandle.hpp"

#include <pango/pango.h>

using namespace PageComposer;

static const ParaStyle s_para_default;
static const TextStyle s_text_default;

inline static const PangoFontDescription* font(const Handle<Paragraph::Font>& f)
	{return reinterpret_cast<const PangoFontDescription*>( f.handle() );}

inline static PangoFontDescription* font(Handle<Paragraph::Font>& f)
	{return reinterpret_cast<PangoFontDescription*>( f.handle() );}


inline static const PangoLayout* layout(const Handle<Paragraph::Layout>& f)
	{return reinterpret_cast<const PangoLayout*>( f.handle() );}

inline static PangoLayout* layout(Handle<Paragraph::Layout>& f)
	{return reinterpret_cast<PangoLayout*>(f.handle());}

Paragraph::Paragraph(TextRenderer& tr):m_font(pango_font_description_new())
	,m_layout(pango_layout_new( pangocontext( tr.handle() ) ) )
	{style(s_para_default).style(s_text_default);}

Paragraph::~Paragraph()
	{
	g_object_unref(m_layout.handle());
	pango_font_description_free(font(m_font));
	}
