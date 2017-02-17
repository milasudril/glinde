//@	{"targets":[{"name":"color.hpp","type":"include"}]}

#ifndef PAGECOMPOSER_COLOR_HPP
#define PAGECOMPOSER_COLOR_HPP

namespace PageComposer
	{
	class Color
		{
		public:
			explicit Color(float r, float g, float b, float a):m_data{b,g,r,a}
				{}

			float red() const noexcept
				{return m_data[2];}

			float& red() noexcept
				{return m_data[2];}


			float green() const noexcept
				{return m_data[1];}

			float& green() noexcept
				{return m_data[1];}


			float blue() const noexcept
				{return m_data[0];}

			float& blue() noexcept
				{return m_data[0];}


			float alpha() const noexcept
				{return m_data[3];}

			float& alpha() noexcept
				{return m_data[3];}

		private:
			typedef float v4sf __attribute__ ((vector_size (16)));
			v4sf m_data;
		};
	};

#endif
