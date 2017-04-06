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
//@	{"targets":[{"name":"arraydynamicstl.hpp","type":"include"}]}

#ifndef GLINDE_ARRAYDYNAMICSTL_HPP
#define GLINDE_ARRAYDYNAMICSTL_HPP

#include "arraydynamic.hpp"

namespace Glinde
	{
	template<class T>
	class ArrayDynamicSTL:private ArrayDynamic<T>
		{
		public:
			typedef T value_type;
			typedef T& reference;
			typedef const T& const_reference;
			typedef uint32_t size_type;

			using ArrayDynamic<T>::ArrayDynamic;
			using ArrayDynamic<T>::operator[];
			using ArrayDynamic<T>::begin;
			using ArrayDynamic<T>::end;
			using ArrayDynamic<T>::operator=;

			size_t size() const noexcept
				{return ArrayDynamic<T>::length();}

			const_reference front() const noexcept
				{return *begin();}

			reference front() noexcept
				{return *begin();}

			const_reference back() const noexcept
				{return *(end() - 1);}

			reference back() noexcept
				{return *(end() - 1);}

			void pop_back() noexcept
				{ArrayDynamic<T>::truncate();}

			void push_back(const T& obj)
				{ArrayDynamic<T>::append(obj);}

			void push_back(T&& obj)
				{ArrayDynamic<T>::append(std::move(obj));}
		};
	};

#endif
