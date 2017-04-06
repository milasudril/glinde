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
//@	{"targets":[{"name":"range.hpp","type":"include"}]}

#ifndef GLINDE_RANGE_HPP
#define GLINDE_RANGE_HPP
#include <cassert>
#include <cstddef>
#include <type_traits>

namespace Glinde
	{
	template<class T>
	class Range
		{
		public:
			typedef typename std::add_pointer<T>::type iterator;

			constexpr Range():Range(nullptr,0){}

			explicit constexpr Range(iterator begin,size_t l):
				r_begin(begin),m_length(l)
				{}

			explicit constexpr Range(iterator begin,iterator end):
				r_begin(begin),m_length(static_cast<size_t>(end-begin))
				{}

			constexpr iterator begin() const noexcept
				{return r_begin;}

			constexpr iterator end() const noexcept
				{return r_begin+m_length;}

			constexpr size_t length() const noexcept
				{return m_length;}

			T& operator[](size_t k) const noexcept
				{
				assert(k<m_length);
				return r_begin[k];
				}

		private:
			iterator r_begin;
			size_t m_length;
		};
	};

#endif
