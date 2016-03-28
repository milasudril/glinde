#ifdef __WAND__
target[name[narrow_cast.h] type[include]]
#endif

#ifndef GLINDA_NARROWCAST_H
#define GLINDA_NARROWCAST_H

#include "errormessage.h"
#include <limits>

namespace Glinda
	{
	template<class T,class U>
	T narrow_cast(const U& x)
		{
		if(x>std::numeric_limits<T>::max() || x<std::numeric_limits<T>::min())
			{throw ErrorMessage("A quantity is outside its valid range.");}

		return static_cast<T>(x);
		}
	}

#endif
