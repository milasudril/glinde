//@	{"targets":[{"name":"rectangle.hpp","type":"include"}]}

#ifndef PAGECOMPOSER_RECTANGLE_HPP
#define PAGECOMPOSER_RECTANGLE_HPP

#include "vec2.hpp"

namespace PageComposer
	{
	class Rectangle
		{
		public:
			Rectangle(){}

			Rectangle(Vec2 min,Vec2 size):m_min(min),m_size(size)
				{}

			Rectangle(double x, double y, double w, double h):m_min{x,y},m_size{w,h}
				{}

			double x() const noexcept
				{return m_min.x();}

			double& x() noexcept
				{return m_min.x();}

			double y() const noexcept
				{return m_min.y();}

			double& y() noexcept
				{return m_min.y();}

			Vec2 min() const noexcept
				{return m_min;}

			Vec2& min() noexcept
				{return m_min;}



			double width() const noexcept
				{return m_size.x();}

			double& width() noexcept
				{return m_size.x();}

			double height() const noexcept
				{return m_size.y();}

			double& height() noexcept
				{return m_size.y();}

			Vec2 size() const noexcept
				{return m_size;}

			Vec2& size() noexcept
				{return m_size;}

		private:
			Vec2 m_min;
			Vec2 m_size;
		};
	};

#endif
