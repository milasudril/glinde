#ifdef __WAND__
target[name[arrayfixed.h] type[include]]
#endif

#ifndef GLINDA_ARRAYFIXED_H
#define GLINDA_ARRAYFIXED_H

#include "range.h"
#include <cstddef>
#include <initializer_list>

namespace Glinda
	{
	template<class T,size_t N>
	class ArrayFixed
		{
		public:
			constexpr ArrayFixed(){}

			template<typename... U>
			constexpr ArrayFixed(U... values):data{values...}
				{}

			constexpr const T& operator[](size_t i) const
				{return data[i];}

			T& operator[](size_t i)
				{return data[i];}

			constexpr const T* begin() const
				{return data;}

			constexpr const T* end() const
				{return data+N;}

			T* begin()
				{return data;}

			T* end()
				{return data+N;}

			static constexpr size_t length()
				{return N;}

			constexpr operator Range<const T*>() const noexcept
				{return Range<const T*>{data,N};}

		private:
			T data[N];
		};
	}

#endif
