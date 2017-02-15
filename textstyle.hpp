//@	{"targets":[{"name":"textstyle.hpp","type":"include"}]}

#ifndef PAGECOMPOSER_TEXTSTYLE_HPP
#define PAGECOMPOSER_TEXTSTYLE_HPP

#include "color.hpp"
#include <string>

namespace PageComposer
	{
	class TextStyle
		{
		public:
			enum class Alignment:int{LEFT,RIGHT,CENTER};

			enum class FontStyle:int{NORMAL,ITALIC,OBLIQUE};

			TextStyle():m_family("serif"),m_style(FontStyle::NORMAL),m_size(16.0)
				,m_weight(0),m_color{0,0,0,1},m_alignment(Alignment::LEFT),m_flags(0)
				{}

			TextStyle& family(const char* fam)
				{
				m_family=fam;
				return *this;
				}

			const char* family() const noexcept
				{return m_family.c_str();}

			TextStyle& style(FontStyle style_new) noexcept
				{
				m_style=style_new;
				return *this;
				}

			FontStyle style() const noexcept
				{return m_style;}

			TextStyle& alignment(Alignment align) noexcept
				{
				m_alignment=align;
				return *this;
				}

			TextStyle& size(float size_new) noexcept
				{
				m_size=size_new;
				return *this;
				}

			float size() const noexcept
				{return m_size;}

			TextStyle& sizePageRelative() noexcept
				{
				m_flags|=SIZE_PAGE_RELATIVE;
				return *this;
				}

			TextStyle& sizeAbsolute() noexcept
				{
				m_flags&=~SIZE_PAGE_RELATIVE;
				return *this;
				}

			bool sizePageRelative() const noexcept
				{return m_flags&SIZE_PAGE_RELATIVE;}

			TextStyle& variantSmallcaps() noexcept
				{
				m_flags|=SMALLCAPS;
				return *this;
				}

			TextStyle& variantNormal() noexcept
				{
				m_flags&=~SMALLCAPS;
				return *this;
				}

			bool smallcaps() const noexcept
				{return m_flags&SMALLCAPS;}

			TextStyle& weight(float value) noexcept
				{
				m_weight=value;
				return *this;
				}

			float weight() const noexcept
				{return m_weight;}


			TextStyle& color(Color c) noexcept
				{
				m_color=c;
				return *this;
				}

			Color color() const noexcept
				{return m_color;}


		private:
			std::string m_family;
			FontStyle m_style;
			float m_size;
			float m_weight;
			Color m_color;
			Alignment m_alignment;
			static constexpr unsigned int SIZE_PAGE_RELATIVE=0x1;
			static constexpr unsigned int SMALLCAPS=0x2;
			unsigned int m_flags;
		};
	}

#endif
