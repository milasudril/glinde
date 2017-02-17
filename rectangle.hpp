//@	{"targets":[{"name":"rectangle.hpp","type":"include"}]}

#ifndef PAGECOMPOSER_RECTANGLE_HPP
#define PAGECOMPOSER_RECTANGLE_HPP

#include "vec2.hpp"
#include <cstddef>

namespace PageComposer
	{
	class Rectangle
		{
		public:
			Rectangle(){}

			explicit Rectangle(Vec2 min,Vec2 max):m_min(min),m_max(max)
				{}


			Vec2 min() const noexcept
				{return m_min;}

			Vec2& min() noexcept
				{return m_min;}

			Vec2 max() const noexcept
				{return m_max;}

			Vec2& max() noexcept
				{return m_max;}

			Vec2 size() const noexcept
				{return max() - min();}

			double area() const noexcept
				{
				auto s=size();
				return s.x()*s.y();
				}

		private:
			Vec2 m_min;
			Vec2 m_max;
		};
	};

#endif
