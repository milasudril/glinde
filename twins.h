#ifdef __WAND__
target[name[twins.h] type[include]]
#endif

#ifndef GLINDA_TWINS_H
#define GLINDA_TWINS_H

#include <utility>

namespace Glinda
	{
	template<class T>
	struct Twins:public std::pair<T,T>
		{
		using std::pair<T,T>::pair;
		};
	};

#endif
