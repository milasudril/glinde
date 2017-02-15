//@	{
//@	 "targets":[{"name":"paragraph.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"paragraph.o","rel":"implementation"}]
//@	}

#ifndef PAGECOMPOSER_PARAGRAPH_HPP
#define PAGECOMPOSER_PARAGRAPH_HPP

#include "handle.hpp"
#include <utility>

namespace PageComposer
	{
	class ParaStyle;
	class TextStyle;

	class Paragraph
		{
		public:
			Paragraph();

			~Paragraph();

			Paragraph(Paragraph&& p) noexcept:
				 r_p_style(p.r_p_style),r_t_style(p.r_t_style)
				,m_font(p.m_font),m_layout(p.m_layout)
				{
				p.m_font.reset();
				p.m_layout.reset();
				}

			Paragraph& operator=(Paragraph&& p) noexcept
				{
				r_p_style=p.r_p_style;
				r_t_style=p.r_t_style;
				std::swap(p.m_font,m_font);
				std::swap(p.m_layout,m_layout);
				return *this;
				}

			Paragraph(const Paragraph& p)=delete;
			Paragraph& operator=(const Paragraph& p)=delete;

			struct Layout{};
			struct Font{};

			Paragraph& style(ParaStyle&& para)=delete;

			Paragraph& style(const ParaStyle& para) noexcept
				{
				r_p_style=&para;
				return *this;
				}

			const ParaStyle& styleParagraph() const noexcept
				{return *r_p_style;}

			Paragraph& style(TextStyle&& t)=delete;

			Paragraph& style(const TextStyle& t) noexcept
				{
				r_t_style=&t;
				return *this;
				}

			const TextStyle& styleText() const noexcept
				{return *r_t_style;}

			Paragraph& textSet(const char* src);
		
		private:
			const ParaStyle* r_p_style;
			const TextStyle* r_t_style;

			Handle<Font> m_font;
			Handle<Layout> m_layout;
		};
	};

#endif
