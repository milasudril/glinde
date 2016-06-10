//@	{
//@	 "targets":[{"name":"world.so","type":"plugin"}]
//@	,"author":["Torbjörn Rathsman"]
//@	,"year":2016
//@	,"license":"Public domain"
//@	,"description":"Sample action program main file"
//@	}

#include "common/hello.h"
#include <glinde/world.h>
#include <glinde/stringkey.h>
#include <cstdio>

using namespace Glinde;

class Init:public World::EventHandler
	{
	public:
		void onLoaded(World& world)
			{
			world.siteSet(Stringkey("map00"));
			printf("Hello!\n");
			}

		void onUnload(World& world)
			{
			printf("Good bye\n");
			delete this;
			}
	};

World::EventHandler& Glinde_World_EventHandler_create()
	{
	return *(new Init);
	}
