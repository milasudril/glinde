//@	{"targets":[{"name":"textstyle.hpp","type":"include"}]}

#ifndef PAGECOMPOSER_TEXTSTYLE_HPP
#define PAGECOMPOSER_TEXTSTYLE_HPP

#include "color.hpp"
#include "textsizemode.hpp"
#include <cstdlib>
#include <cstring>
#include <utility>

namespace PageComposer
	{
	class TextStyle
		{
		public:
			enum class FontStyle:int{NORMAL,ITALIC,OBLIQUE};

			TextStyle():m_family_size(0),m_style(FontStyle::NORMAL),m_size(16.0)
				,m_size_mode(TextSizeMode::ABSOLUTE),m_weight(0.3f),m_color{0,0,0,1},m_flags(0)
				{
				family("Serif");
				}

			~TextStyle() noexcept
				{free(m_family);}

			TextStyle(TextStyle&& obj) noexcept:m_family(obj.m_family)
				,m_family_size(obj.m_family_size)
				,m_style(obj.m_style)
				,m_size(obj.m_size)
				,m_size_mode(obj.m_size_mode)
				,m_weight(obj.m_weight)
				,m_color(obj.m_color)
				,m_flags(obj.m_flags)
				{
				obj.m_family=nullptr;
				obj.m_family_size=0;
				}

			TextStyle& operator=(TextStyle&& obj) noexcept
				{
				std::swap(obj.m_family,m_family);
				std::swap(obj.m_family_size,obj.m_family_size);
				m_style=obj.m_style;
				m_size=obj.m_size;
				m_size_mode=obj.m_size_mode;
				m_weight=obj.m_weight;
				m_color=obj.m_color;
				m_flags=obj.m_flags;
				return *this;
				}

			TextStyle(const TextStyle& obj):
				 m_family_size(0)
				,m_style(obj.m_style)
				,m_size(obj.m_size)
				,m_size_mode(obj.m_size_mode)
				,m_weight(obj.m_weight)
				,m_color(obj.m_color)
				,m_flags(obj.m_flags)
				{family(obj.family());}

			TextStyle& operator=(const TextStyle& obj)
				{
				TextStyle temp(obj);
				*this=std::move(temp);
				return *this;
				}

			TextStyle& family(const char* fam)
				{
				auto l=strlen(fam) + 1;
				if(l>m_family_size)
					{
					auto ptr=static_cast<char*>(malloc(l*sizeof(char)));
					if(ptr==nullptr)
						{throw "Out of memory";}
					m_family=ptr;
					m_family_size=l;
					}
				memcpy(m_family,fam,m_family_size);
				return *this;
				}

			const char* family() const noexcept
				{return m_family;}

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

			TextStyle& textSizeMode(TextSizeMode s) noexcept
				{
				m_size_mode=s;
				return *this;
				}

			TextSizeMode textSizeMode() const noexcept
				{return m_size_mode;}


		private:
			char* m_family;
			size_t m_family_size;
			FontStyle m_style;
			float m_size;
			TextSizeMode m_size_mode;
			float m_weight;
			Color m_color;
			static constexpr unsigned int SMALLCAPS=0x1;
			unsigned int m_flags;
		};
	}

#endif
