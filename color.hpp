//@	{
//@	"targets":[{"name":"color.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"color.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_COLOR_HPP
#define GLINDE_COLOR_HPP

#include "vectortype.hpp"
#include "arrayfixed.hpp"
#include <cmath>
#include <cstdint>

namespace Glinde
	{
	class Color
		{
		public:
			constexpr Color() noexcept:m_data{0,0,0,0}
				{}

			static constexpr auto fromSRGB(vec4_t<float> x) noexcept
				{
				Color ret;
				ret.m_data=vec4_t<float>
					{
					 fromSRGB(x[0])
					,fromSRGB(x[1])
					,fromSRGB(x[2])
					,x[3]
					};
				return ret;
				}

			static constexpr auto fromLinear(vec4_t<float> x) noexcept
				{
				Color ret;
				ret.m_data=x;
				return ret;
				}

			static constexpr auto fromSRGB(vec4_t<uint8_t> x)
				{
				Color ret;
				ret.m_data=vec4_t<float>
					{
					 srgb_lut[x[0]]
					,srgb_lut[x[1]]
					,srgb_lut[x[2]]
					,x[3]/255.0f
					};
				return ret;
				}

			static constexpr float fromSRGB(float x) noexcept
				{return x<=0.04045f? x/12.92f : std::pow( (x + 0.055f)/(1.0f + 0.055f),2.4f);}

			constexpr auto red() const noexcept
				{return m_data[0];}

			constexpr auto green() const noexcept
				{return m_data[1];}

			constexpr auto blue() const noexcept
				{return m_data[2];}

			constexpr auto alpha() const noexcept
				{return m_data[3];}

			constexpr auto& red() noexcept
				{return m_data[0];}

			constexpr auto& green() noexcept
				{return m_data[1];}

			constexpr auto& blue() noexcept
				{return m_data[2];}

			constexpr auto& alpha() noexcept
				{return m_data[3];}

			Color& multiplyByAlpha() noexcept
				{
				auto x=m_data[3];
				m_data*=x;
				return *this;
				}

			Color& divideByAlpha() noexcept
				{
				auto x=m_data[3];
				m_data/=x;
				return *this;
				}

		private:
			vec4_t<float> m_data;
			static const ArrayFixed<float,256> srgb_lut;
		};
	}
#endif
