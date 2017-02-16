//@	{"targets":[{"name":"vec2.hpp","type":"include"}]}

#ifndef PAGECOMPOSER_VEC2_HPP
#define PAGECOMPOSER_VEC2_HPP

namespace PageComposer
	{
	class Vec2
		{
		public:
			typedef double v2sd __attribute__ ((vector_size (16)));

			Vec2() noexcept:m_data{0,0}
				{}

			Vec2(v2sd x) noexcept:m_data(x)
				{}

			Vec2(double x, double y):m_data{x,y}
				{}

			double x() const noexcept
				{return m_data[0];}

			double& x() noexcept
				{return m_data[0];}


			double y() const noexcept
				{return m_data[1];}

			double& y() noexcept
				{return m_data[1];}

			Vec2& operator+=(Vec2 b) noexcept
				{
				m_data+=b.m_data;
				return *this;
				}

			Vec2& operator-=(Vec2 b) noexcept
				{
				m_data-=b.m_data;
				return *this;
				}

			Vec2& hadamard(Vec2 b) noexcept
				{
				m_data*=b.m_data;
				return *this;
				}

			Vec2& operator*=(double c) noexcept
				{
				m_data*=c;
				return *this;
				}

			v2sd data() const noexcept
				{return m_data;}

		private:
			v2sd m_data;
		};

	inline Vec2 operator+(Vec2 a,Vec2 b) noexcept
		{return a+=b;}

	inline Vec2 operator-(Vec2 a,Vec2 b) noexcept
		{return a-=b;}

	inline Vec2 hadamard(Vec2 a,Vec2 b) noexcept
		{return a.hadamard(b);}

	inline Vec2 operator*(double c,Vec2 b) noexcept
		{return b*=c;}

	inline Vec2 emin(Vec2 a,Vec2 b) noexcept
		{return a.data()<b.data()?a.data():b.data();}

	inline Vec2 emax(Vec2 a,Vec2 b) noexcept
		{return b.data()<a.data()?a.data():b.data();}

	};

#endif
