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
