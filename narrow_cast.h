//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"narrow_cast.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_NARROWCAST_H
#define GLINDE_NARROWCAST_H

#include "errormessage.h"
#include "variant.h"
#include <limits>
#include <type_traits>

namespace Glinde
	{
	template<class T,class U>
	typename std::enable_if<std::is_unsigned<T>::value,T>::type
	narrow_cast(const U& x)
		{
		auto ymax=std::numeric_limits<T>::max();
		if(x<0
			|| static_cast<typename std::make_unsigned<T>::type>(x) > ymax)
			{throw ErrorMessage("A quantity is outside its valid range.",{});}

		return static_cast<T>(x);
		}

	template<class T,class U>
	typename std::enable_if<std::is_signed<T>::value,T>::type
	narrow_cast(const U& x)
		{
		if(x>std::numeric_limits<T>::max() || x<std::numeric_limits<T>::min())
			{throw ErrorMessage("A quantity is outside its valid range.",{});}

		return static_cast<T>(x);
		}
	}

#endif
