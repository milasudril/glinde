#ifdef __WAND__
target[name[twins.h] type[include]]
#endif

#ifndef GLINDE_TWINS_H
#define GLINDE_TWINS_H

#include <utility>

namespace Glinde
	{
	template<class T>
	struct Twins:public std::pair<T,T>
		{
		using std::pair<T,T>::pair;
		};
	};

#endif
