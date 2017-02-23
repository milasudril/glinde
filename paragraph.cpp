//@	{
//@	 "targets":
//@		[
//@			{"name":"paragraph.o","type":"object","pkgconfig_libs":["pangocairo"] }
//@		]
//@	}

#include "paragraph.hpp"
#include "parastyle.hpp"
#include "textstyle.hpp"
#include "textrenderer.hpp"
#include "pangohandles.hpp"
#include "cairohandles.hpp"
#include <cassert>
#include <pango/pangocairo.h>

using namespace PageComposer;

static const ParaStyle s_para_default;
static const TextStyle s_text_default;

#define ERROR(x) abort()

Paragraph::Paragraph(TextRenderer& tr):PageObjectBase(tr.renderContext())
	,m_flags(0xf)
	,m_font(pango_font_description_new())
	,m_layout(pango_layout_new( pangocontext( tr.handle() ) ) )
	{style(s_para_default).style(s_text_default);}

Paragraph::~Paragraph()
	{
	g_object_unref(m_layout.handle());
	pango_font_description_free(font(m_font));
	}

Paragraph& Paragraph::text(const char* src)
	{
	dirty_set();
	pango_layout_set_text(layout(m_layout),src,-1);
	return *this;
	}

Rectangle Paragraph::bounding_rectangle_raw() const noexcept
	{
	if(m_flags&STYLE_DIRTY)
		{style_apply();}

	auto handle=const_cast<PangoLayout*>(layout(m_layout));
	PangoRectangle ink;
	pango_layout_get_pixel_extents(handle,&ink,NULL);
	Vec2 min(ink.x,ink.y);
	auto max=min+Vec2(ink.width,ink.height);
	return Rectangle{min,max};
	}


static PangoAlignment pango_alignment(ParaStyle::Alignment a) noexcept
	{
	switch(a)
		{
		case ParaStyle::Alignment::LEFT:
			return PANGO_ALIGN_LEFT;
		case ParaStyle::Alignment::CENTER:
			return PANGO_ALIGN_CENTER;
		case ParaStyle::Alignment::RIGHT:
			return PANGO_ALIGN_RIGHT;
		}
	return PANGO_ALIGN_LEFT;
	}

static constexpr PangoWeight weights[]=
	{
	 PANGO_WEIGHT_THIN
	,PANGO_WEIGHT_ULTRALIGHT
	,PANGO_WEIGHT_LIGHT
	,PANGO_WEIGHT_NORMAL
	,PANGO_WEIGHT_MEDIUM
	,PANGO_WEIGHT_SEMIBOLD
	,PANGO_WEIGHT_BOLD
	,PANGO_WEIGHT_ULTRABOLD
	,PANGO_WEIGHT_HEAVY
	,PANGO_WEIGHT_ULTRAHEAVY
	};

static PangoWeight pango_font_weight(float weight) noexcept
	{
	auto v=static_cast<int>(0.5f + 9.0f*weight);
	v=std::max(0,std::min(v,9));
	return weights[v];
	}

static PangoStyle pango_style(TextStyle::FontStyle style) noexcept
	{
	switch(style)
		{
		case TextStyle::FontStyle::NORMAL:
			return PANGO_STYLE_NORMAL;
		case TextStyle::FontStyle::ITALIC:
			return PANGO_STYLE_ITALIC;
		case TextStyle::FontStyle::OBLIQUE:
			return PANGO_STYLE_OBLIQUE;
		}
	return PANGO_STYLE_NORMAL;
	}


static float size_box(const ParaStyle& para,const TextStyle& font,int w,int h);

static float size_font(const ParaStyle& para,const TextStyle& font,int w,int h)
	{
	auto size=font.size();
	switch(font.textSizeMode())
		{
		case TextSizeMode::ABSOLUTE:
			return size;
		case TextSizeMode::FONT:
			return size;
		case TextSizeMode::BOX:
			if(para.textSizeMode()==TextSizeMode::FONT)
				{ERROR("Inconsistent size constraint");}
			return size_box(para,font,w,h)*size;
		case TextSizeMode::PAGE:
			return size*h;
		}
	return size;
	}

static float size_box(const ParaStyle& para,const TextStyle& font,int w,int h)
	{
	auto size=para.size();
	switch(para.textSizeMode())
		{
		case TextSizeMode::ABSOLUTE:
			return size;
		case TextSizeMode::FONT:
			if(font.textSizeMode()==TextSizeMode::BOX)
				{ERROR("Inconsistent size constraint");}
			return size_font(para,font,w,h)*size;
		case TextSizeMode::BOX:
			return size;
		case TextSizeMode::PAGE:
			return para.sizeDimension()==ParaStyle::SizeDimension::HEIGHT?
				size*h:size*w;
		}
	return size;
	}

void Paragraph::style_apply() const noexcept
	{
	auto width=render_context().surface().width();
	auto height=render_context().surface().height();

	auto f=const_cast<PangoFontDescription*>(font(m_font));
	pango_font_description_set_family(f, m_t_style.family() );
	pango_font_description_set_weight(f, pango_font_weight(m_t_style.weight()) );
	pango_font_description_set_style(f, pango_style(m_t_style.style() ) );
	pango_font_description_set_variant(f
		,m_t_style.smallcaps()?PANGO_VARIANT_SMALL_CAPS:PANGO_VARIANT_NORMAL);
	pango_font_description_set_size(f,size_font(m_p_style,m_t_style,width,height)*PANGO_SCALE);

	auto para_size_dim=m_p_style.sizeDimension();
	auto l=const_cast<PangoLayout*>(layout(m_layout));
	pango_layout_set_font_description(l,f);
	if(para_size_dim==ParaStyle::SizeDimension::HEIGHT)
		{pango_layout_set_height(l,size_box(m_p_style,m_t_style,width,height)*PANGO_SCALE);}
	else
		{pango_layout_set_width(l,size_box(m_p_style,m_t_style,width,height)*PANGO_SCALE);}
	pango_layout_set_alignment(l,pango_alignment(m_p_style.alignment()));

	m_flags&=~STYLE_DIRTY;
	}

void Paragraph::render_impl() const noexcept
	{
	auto rc_handle=cairocontext(render_context().handle());
	cairo_set_source_rgba( rc_handle
		,m_p_style.color().red()
		,m_p_style.color().green()
		,m_p_style.color().blue()
		,m_p_style.color().alpha() );

//	bounding_rectangle_raw will update styles if needed
	auto target_rect=bounding_rectangle_raw();
	auto size=target_rect.size();
	auto pos_rect=positionAbsolute()-0.5*hadamard(size,anchor() + Vec2{1,1});
	auto pos_text=pos_rect - target_rect.min();

	cairo_rectangle(rc_handle,pos_rect.x(),pos_rect.y()
		,size.x(),size.y());
	cairo_fill(rc_handle);

	cairo_move_to(rc_handle,pos_text.x(),pos_text.y());
	cairo_set_source_rgba( rc_handle
		,m_t_style.color().red()
		,m_t_style.color().green()
		,m_t_style.color().blue()
		,m_t_style.color().alpha());
	pango_cairo_show_layout(rc_handle
		,const_cast<PangoLayout*>(layout(m_layout)));

	m_bounding_rect_old=boundingRectangle();
	}
