//@	{
//@	 "targets":[{"name":"world.so","type":"plugin"}]
//@	,"author":["Torbjörn Rathsman"]
//@	,"year":2016
//@	,"license":"Public domain"
//@	,"description":"Sample action program main file"
//@	}

#include "common/hello.h"
#include <glinde/engine.h>

using namespace Glinde;

extern "C"
	{
	Engine::EventHandler* eventHandlerCreate()
		{
		hello();
		return nullptr;
		}
	}
