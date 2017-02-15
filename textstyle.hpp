//@	{"targets":[{"name":"textstyle.hpp","type":"include"}]}

#ifndef PAGECOMPOSER_TEXTSTYLE_HPP
#define PAGECOMPOSER_TEXTSTYLE_HPP

#include "color.hpp"
#include "sizebase.hpp"
#include <string>

namespace PageComposer
	{
	class TextStyle
		{
		public:
			enum class FontStyle:int{NORMAL,ITALIC,OBLIQUE};

			TextStyle():m_family("serif"),m_style(FontStyle::NORMAL),m_size(16.0)
				,m_size_base(SizeBase::ABSOLUTE),m_weight(0.3f),m_color{0,0,0,1},m_flags(0)
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

			TextStyle& size(float size_new) noexcept
				{
				m_size=size_new;
				return *this;
				}

			float size() const noexcept
				{return m_size;}

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

			TextStyle& sizeBase(SizeBase s) noexcept
				{
				m_size_base=s;
				return *this;
				}

			SizeBase sizeBase() const noexcept
				{return m_size_base;}


		private:
			std::string m_family;
			FontStyle m_style;
			float m_size;
			SizeBase m_size_base;
			float m_weight;
			Color m_color;
			static constexpr unsigned int SMALLCAPS=0x1;
			unsigned int m_flags;
		};
	}

#endif
