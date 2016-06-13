#ifdef __WAND__
target[name[maptest] type[application]]
#endif

#include "mapfixed.h"
#include "stringkey.h"
#include <cstdio>
#include <cmath>

using namespace Glinde;

typedef MapFixed<Stringkey::HashValue,float
		,Stringkey("pi")
		,Stringkey("e")
		,Stringkey("Phi")
		,Stringkey("C")> MapType;

void __attribute__ ((noinline)) valuePrint(const MapType& map,const Stringkey& key)
	{
	printf("Key has value %.8g\n",map[key]);
	}

int main()
	{
	MapType map;


	map.get<Stringkey("pi")>()=std::acos(-1.0f);
	map.get<Stringkey("e")>()=std::log(1.0f);
	map.get<Stringkey("Phi")>()=0.5f*(1.0f + std::sqrt(5.0f));


	constexpr Stringkey x("Phi");
	valuePrint(map,x);



	return 0;
	}
