//@	{"targets":[{"name":"parastyle.hpp","type":"include"}]}

#ifndef PAGECOMPOSER_PARASTYLE_HPP
#define PAGECOMPOSER_PARASTYLE_HPP

#include "color.hpp"
#include <string>

namespace PageComposer
	{
	class ParaStyle
		{
		public:
			enum class Alignment:int{LEFT,RIGHT,CENTER};

			ParaStyle():m_size(16.0),m_color{1,1,1,0.75}
				,m_alignment(Alignment::LEFT)
				{}

			ParaStyle& alignment(Alignment align) noexcept
				{
				m_alignment=align;
				return *this;
				}

			ParaStyle& size(float size_new) noexcept
				{
				m_size=size_new;
				return *this;
				}

			float size() const noexcept
				{return m_size;}

			ParaStyle& color(Color c) noexcept
				{
				m_color=c;
				return *this;
				}

			Color color() const noexcept
				{return m_color;}


		private:
			float m_size;
			Color m_color;
			Alignment m_alignment;
		};
	}

#endif
