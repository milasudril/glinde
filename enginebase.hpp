/*
Glinde
Copyright (C) 2017  Torbjörn Rathsman

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/
//@	{"targets":[{"name":"enginebase.hpp","type":"include"}]}

#ifndef GLINDE_ENGINEBASE_HPP
#define GLINDE_ENGINEBASE_HPP

#include <memory>

namespace Glinde
	{
	class Timer;
	class GameLoader;
	class Timeinfo;
	class Game;
	class EngineBase
		{
		public:
			virtual ~EngineBase()=default;
			virtual void stop()=0;
			virtual void run(Timer& timer)=0;
			virtual void consoletest()=0;
			virtual void gameLoad(const char* archive)=0;
			virtual void operator()(const Timeinfo& ti,GameLoader* loader,int x)=0;
			virtual void operator()(const Timeinfo& ti,std::unique_ptr<Game>&& game,int x)=0;
		};
	}

#endif