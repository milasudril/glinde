//@	{
//@	 "targets":[{"name":"paragraph.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"paragraph.o","rel":"implementation"}]
//@	}

#ifndef PAGECOMPOSER_PARAGRAPH_HPP
#define PAGECOMPOSER_PARAGRAPH_HPP

#include "handle.hpp"

namespace PageComposer
	{
	class ParaStyle;
	class TextStyle;

	class Paragraph
		{
		public:
			Paragraph();
			~Paragraph();

			Paragraph(Paragraph&& p) noexcept;

			Paragraph& operator=(Paragraph&& p) noexcept
				{
				return *this;
				}

			Paragraph(const Paragraph& p)=delete;
			Paragraph& operator=(const Paragraph& p)=delete;


			struct Layout{};
			struct Font{};
		
		private:
			const ParaStyle* r_p_style;
			const TextStyle* r_t_style;

			Handle<Font> m_font;
			Handle<Layout> m_layout;
		};
	};

#endif
