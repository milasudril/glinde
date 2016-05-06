#ifdef __WAND__
target[name[range.h] type[include]]
#endif

#ifndef GLINIE_RANGE_H
#define GLINIE_RANGE_H

#include <cstddef>

namespace Glinie
	{
	template<class T>
	class Range
		{
		public:
			explicit constexpr Range(T begin,size_t l):
				r_begin(begin),m_length(l)
				{}

			constexpr T begin() noexcept
				{return r_begin;}

			constexpr T end() noexcept
				{return r_begin+m_length;}

			constexpr size_t length() const noexcept
				{return m_length;}

		private:
			T r_begin;
			size_t m_length;
		};
	};

#endif
