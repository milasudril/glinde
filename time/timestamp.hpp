/*
Glinde
Copyright (C) 2017  Torbjörn Rathsman

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/
//@	{"targets":[{"name":"timestamp.hpp","type":"include"}]}

#ifndef GLINDE_TIMESTAMP_HPP
#define GLINDE_TIMESTAMP_HPP

#include <cstdint>

namespace Glinde
	{
	/**A Timestamp is date, packed into a 64-bit word. The smallest
	 * quantity that can be represented by a Timestamp is one second.
	*/
	class Timestamp
		{
		public:
			Timestamp():m_bits(0){}
			
			bool operator<(const Timestamp& t_prime) const noexcept
				{return m_bits < t_prime.m_bits;}

			bool operator==(const Timestamp& t_prime) const noexcept
				{return m_bits==t_prime.m_bits;}

			bool operator!=(const Timestamp& t_prime) const noexcept
				{return !(*this==t_prime);}

			bool operator>(const Timestamp& t_prime) const noexcept
				{return m_bits>t_prime.m_bits;}

			bool operator<=(const Timestamp& t_prime) const noexcept
				{return !(*this>t_prime);}

			bool operator>=(const Timestamp& t_prime) const noexcept
				{return !(*this < t_prime);}

			auto sec() const noexcept
				{return m_bits&MASK_SEC;}

			Timestamp& sec(uint32_t sec) noexcept
				{
				uint64_t bits_temp=m_bits
					&(MASK_MIN|MASK_HOUR|MASK_DAY|MASK_MONTH|MASK_YEAR);
				m_bits=bits_temp|(sec&0x3f);
				return *this;
				}

			auto min() const noexcept
				{return (m_bits&MASK_MIN)>>6;}

			Timestamp& min(uint32_t min) noexcept
				{
				uint64_t bits_temp=m_bits
					&(MASK_SEC|MASK_HOUR|MASK_DAY|MASK_MONTH|MASK_YEAR);
				m_bits=bits_temp|((min&0x3f)<<6);
				return *this;
				}

			auto hour() const noexcept
				{return (m_bits&MASK_HOUR)>>12;}

			Timestamp& hour(uint32_t hour) noexcept
				{
				uint64_t bits_temp=m_bits
					&(MASK_SEC|MASK_MIN|MASK_DAY|MASK_MONTH|MASK_YEAR);
				m_bits=bits_temp|((hour&0x1f)<<12);
				return *this;
				}

			auto day() const noexcept
				{return (m_bits&MASK_DAY)>>17;}

			Timestamp& day(uint32_t day)
				{
				uint64_t bits_temp=m_bits
					&(MASK_SEC|MASK_MIN|MASK_HOUR|MASK_MONTH|MASK_YEAR);
				m_bits=bits_temp|((day&0x1f)<<17);
				return *this;
				}

			auto month() const
				{return (m_bits&MASK_MONTH)>>22;}

			Timestamp& month(uint32_t month) noexcept
				{
				uint64_t bits_temp=m_bits
					&(MASK_SEC|MASK_MIN|MASK_HOUR|MASK_DAY|MASK_YEAR);
				m_bits=bits_temp|((month&0xf)<<22);
				return *this;
				}

			int64_t year() const
				{
				uint64_t ret=(m_bits&MASK_YEAR)>>26;
				if(ret&0x2000000000LLU)
					{ret|=(0x3ffffffLLU<<38);}
				return ret;
				}

			Timestamp& year(int64_t year) noexcept
				{
				uint64_t bits_temp=m_bits
					&(MASK_SEC|MASK_MIN|MASK_HOUR|MASK_DAY|MASK_MONTH);
				m_bits=bits_temp|((year&0x3fffffffffLL)<<26);
				return *this;
				}

			static constexpr uint64_t YEAR_MAX=0x1fffffffffLLU;
			static constexpr uint64_t YEAR_MIN=0x2000000000LLU;

		private:
			static constexpr uint64_t MASK_SEC=0x3f;
			static constexpr uint64_t MASK_MIN=0x3f<<6;
			static constexpr uint64_t MASK_HOUR=0x1f<<12;
			static constexpr uint64_t MASK_DAY=0x1f<<17;
			static constexpr uint64_t MASK_MONTH=0xf<<22;
			static constexpr uint64_t MASK_YEAR=0x3fffffffffLLU<<26;
			uint64_t m_bits;
		};
	}
#endif
