#ifdef __WAND__
target[name[twins.h] type[include]]
#endif

#ifndef GLINIE_TWINS_H
#define GLINIE_TWINS_H

#include <utility>

namespace Glinie
	{
	template<class T>
	struct Twins:public std::pair<T,T>
		{
		using std::pair<T,T>::pair;
		};
	};

#endif
