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
#include "rendercontext.hpp"
#include "surface.hpp"
#include <cassert>

using namespace PageComposer;

static const ParaStyle s_para_default;
static const TextStyle s_text_default;

#define ERROR(x) abort()

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
	switch(font.sizeBase())
		{
		case SizeBase::ABSOLUTE:
			return size;
		case SizeBase::FONT:
			return size;
		case SizeBase::BOX:
			if(para.sizeBase()==SizeBase::FONT)
				{ERROR("Inconsistent size constraint");}
			return size_box(para,font,w,h)*size;
		case SizeBase::PAGE:
			return size*h;
		}
	return size;
	}

static float size_box(const ParaStyle& para,const TextStyle& font,int w,int h)
	{
	auto size=para.size();
	switch(para.sizeBase())
		{
		case SizeBase::ABSOLUTE:
			return size;
		case SizeBase::FONT:
			if(font.sizeBase()==SizeBase::BOX)
				{ERROR("Inconsistent size constraint");}
			return size_font(para,font,w,h)*size;
		case SizeBase::BOX:
			return size;
		case SizeBase::PAGE:
			return para.sizeDimension()==ParaStyle::SizeDimension::HEIGHT?
				size*h:size*w;
		}
	return size;
	}

void Paragraph::styleApply(const RenderContext& rc) const noexcept
	{
	auto width=rc.surface().width();
	auto height=rc.surface().height();

	auto f=const_cast<PangoFontDescription*>(font(m_font));
	pango_font_description_set_family(f, r_t_style->family() );
	pango_font_description_set_weight(f, pango_font_weight(r_t_style->weight()) );
	pango_font_description_set_style(f, pango_style(r_t_style->style() ) );
	pango_font_description_set_variant(f
		,r_t_style->smallcaps()?PANGO_VARIANT_SMALL_CAPS:PANGO_VARIANT_NORMAL);
	pango_font_description_set_size(f,size_font(*r_p_style,*r_t_style,width,height)*PANGO_SCALE);


	auto para_size_dim=r_p_style->sizeDimension();
	auto l=const_cast<PangoLayout*>(layout(m_layout));
	if(para_size_dim==ParaStyle::SizeDimension::HEIGHT)
		{pango_layout_set_height(l,size_box(*r_p_style,*r_t_style,width,height)*PANGO_SCALE);}
	else
		{pango_layout_set_width(l,size_box(*r_p_style,*r_t_style,width,height)*PANGO_SCALE);}


	pango_layout_set_alignment(l,pango_alignment(r_p_style->alignment()));
	}
