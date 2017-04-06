//@	{
//@	"targets":[{"name":"wallclocktime.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"wallclocktime.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_WALLCLOCKTIME_HPP
#define GLINDE_WALLCLOCKTIME_HPP

#include "timestamp.hpp"

namespace Glinde
	{
	class WallclockTime
		{
		public:
			WallclockTime() noexcept;
			Timestamp timestampUTC() const noexcept;
			Timestamp timestampLocal() const noexcept;
		
		private:
			int64_t m_value;
		};
	}

#endif
