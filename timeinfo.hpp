//@	{"targets":[{"name":"timeinfo.hpp","type":"include"}]}

#ifndef GLINDE_TIMEINFO_HPP
#define GLINDE_TIMEINFO_HPP

#include "vectortype.hpp"
#include <cstddef>

namespace Glinde
	{
	class Timeinfo
		{
		public:
			Timeinfo(double t,double dt,int64_t wallclock) noexcept:
				m_t{t,dt},m_wc{wallclock,0}
				{}

			double t() const noexcept
				{return m_t[0];}

			double dt() const noexcept
				{return m_t[1];}

			int64_t wallclock() const noexcept
				{return m_wc[0];}
		

		private:
			vec2_t<double> m_t;
			vec2_t<int64_t> m_wc;
		};
	}

#endif
