//@	{
//@	 "targets":[{"name":"paragraph.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"paragraph.o","rel":"implementation"}]
//@	}

#ifndef PAGECOMPOSER_PARAGRAPH_HPP
#define PAGECOMPOSER_PARAGRAPH_HPP

#include "handle.hpp"
#include "rectangle.hpp"
#include "parastyle.hpp"
#include "textstyle.hpp"
#include "rendercontext.hpp"
#include "surface.hpp"
#include "vec2.hpp"
#include <utility>

namespace PageComposer
	{
	class ParaStyle;
	class TextStyle;
	class TextRenderer;

	class Paragraph
		{
		public:
			Paragraph(TextRenderer& tr);

			~Paragraph();

			Paragraph(Paragraph&& p) noexcept:m_flags(p.m_flags),r_rc(p.r_rc)
				,m_p_style(std::move(p.m_p_style)),m_t_style(std::move(p.m_t_style))
				,m_font(p.m_font),m_layout(p.m_layout)
				{
				p.m_font.reset();
				p.m_layout.reset();
				}

			Paragraph& operator=(Paragraph&& p) noexcept
				{
				m_flags=p.m_flags;
				r_rc=p.r_rc;
				std::swap(m_p_style,p.m_p_style);
				std::swap(m_t_style,p.m_t_style);
				std::swap(p.m_font,m_font);
				std::swap(p.m_layout,m_layout);
				return *this;
				}

			Paragraph(const Paragraph& p)=delete;
			Paragraph& operator=(const Paragraph& p)=delete;


			Paragraph& style(const ParaStyle& para) noexcept
				{
				m_p_style=para;
				m_flags|=STYLE_DIRTY;
				return *this;
				}

			const ParaStyle& styleParagraph() const noexcept
				{return m_p_style;}

			Paragraph& style(const TextStyle& t) noexcept
				{
				m_t_style=t;
				m_flags|=STYLE_DIRTY;
				return *this;
				}

			const TextStyle& styleText() const noexcept
				{return m_t_style;}

			Paragraph& positionAbsolute(Vec2 pos) noexcept
				{
				m_pos=pos;
				m_flags|=CONTENT_DIRTY;
				return *this;
				}

			Paragraph& positionRelative(Vec2 pos) noexcept
				{
				Vec2 size{double(r_rc->surface().width()),double(r_rc->surface().height())};
				return positionAbsolute( 0.5*hadamard(size,pos + Vec2{1,1}) );
				}

			Vec2 positionAbsolute() const noexcept
				{return m_pos;}

			Vec2 positionRelative() const noexcept
				{return m_pos;}


			Paragraph& anchor(Vec2 a) noexcept
				{
				m_anchor=a;
				m_flags|=CONTENT_DIRTY;
				return *this;
				}

			Vec2 anchor() const noexcept
				{return m_anchor;}



			Paragraph& text(const char* src);

			Rectangle boundingRectangle() const noexcept;

			void render() const noexcept
				{
				if( (m_flags&STYLE_DIRTY) || (m_flags&CONTENT_DIRTY) )
					{render_impl();}
				}
		
		private:
			void render_impl() const noexcept;
			void styleApply() const noexcept;

			mutable unsigned int m_flags;
			static constexpr unsigned int STYLE_DIRTY=0x2;
			static constexpr unsigned int CONTENT_DIRTY=0x1;
			RenderContext* r_rc;
			Vec2 m_pos;
			Vec2 m_anchor;
			ParaStyle m_p_style;
			TextStyle m_t_style;
			Handle<font_t> m_font;
			Handle<layout_t> m_layout;

		};
	};

#endif
