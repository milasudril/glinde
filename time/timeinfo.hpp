//@	{"targets":[{"name":"timeinfo.hpp","type":"include"}]}

#ifndef GLINDE_TIMEINFO_HPP
#define GLINDE_TIMEINFO_HPP

#include "wallclocktime.hpp"
#include "../vectortype.hpp"
#include <cstddef>

namespace Glinde
	{
	class Timeinfo
		{
		public:
			explicit Timeinfo(double t,double dt) noexcept:
				m_t{t,dt}
				{}

			double simulationTime() const noexcept
				{return m_t[0];}

			double dt() const noexcept
				{return m_t[1];}

			WallclockTime wallclockTime() const noexcept
				{return m_wct;}
		

		private:
			vec2_t<double> m_t;
			WallclockTime m_wct;
		};
	}

#endif
