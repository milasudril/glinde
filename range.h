//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"range.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_RANGE_H
#define GLINDE_RANGE_H
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
