//@	{
//@	 "targets":[{"name":"paragraph.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"paragraph.o","rel":"implementation"}]
//@	}

#ifndef PAGECOMPOSER_PARAGRAPH_HPP
#define PAGECOMPOSER_PARAGRAPH_HPP

#include "parastyle.hpp"
#include "textstyle.hpp"
#include "rendercontext.hpp"
#include "surface.hpp"
#include "pageobjectbase.hpp"
#include <utility>

namespace PageComposer
	{
	class ParaStyle;
	class TextStyle;
	class TextRenderer;

	class Paragraph:public PageObjectBase
		{
		public:
			explicit Paragraph(TextRenderer& tr);

			~Paragraph();

			Paragraph(Paragraph&& p) noexcept:PageObjectBase(std::move(p))	
				,m_flags(p.m_flags)
				,m_p_style(std::move(p.m_p_style)),m_t_style(std::move(p.m_t_style))
				,m_font(p.m_font),m_layout(p.m_layout)
				{
				p.m_font.reset();
				p.m_layout.reset();
				}

			Paragraph& operator=(Paragraph&& p) noexcept
				{
				m_flags=p.m_flags;
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
				dirty_set();
				return *this;
				}

			const ParaStyle& styleParagraph() const noexcept
				{return m_p_style;}

			Paragraph& style(const TextStyle& t) noexcept
				{
				m_t_style=t;
				m_flags|=STYLE_DIRTY;
				dirty_set();
				return *this;
				}

			const TextStyle& styleText() const noexcept
				{return m_t_style;}



			Paragraph& text(const char* src);

			Rectangle boundingRectangle() const noexcept
				{
				auto target_rect=bounding_rectangle_raw(); 
				auto size=target_rect.size();
				auto pos_rect=positionAbsolute()-0.5*hadamard(size,anchor() + Vec2{1,1});
				return Rectangle{pos_rect,size + pos_rect};
				}

			void render() const noexcept
				{
				render_impl();
				dirty_clear();
				}
		
		private:
			Rectangle bounding_rectangle_raw() const noexcept;

			void render_impl() const noexcept;
			void style_apply() const noexcept;

			mutable unsigned int m_flags;
			static constexpr unsigned int STYLE_DIRTY=0x2;

			ParaStyle m_p_style;
			TextStyle m_t_style;
			Handle<font_t> m_font;
			Handle<layout_t> m_layout;

		};
	};

#endif
