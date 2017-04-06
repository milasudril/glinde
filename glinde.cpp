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
//@	{"targets":[{"name":"glinde","type":"application"}]}

#include "engine.hpp"
#include "errormessage.hpp"
#include "time/timerdummy.hpp"
#include "time/timerreal.hpp"
#include "log/logwriter.hpp"

#ifdef PROFILE
inline Glinde::TimerDummy timerCreate(double rate)
	{return std::move(Glinde::TimerDummy(rate));}
#else
inline Glinde::TimerReal timerCreate(double rate)
	{return std::move(Glinde::TimerReal(rate));}
#endif


int main()
	{
	try
		{
		Glinde::Engine engine;
		engine.run(timerCreate(30));

	//TODO:
	//	check argv
	//		If a world is going to be loaded
	//			Load that world and move input control to its menu
	//		Else
	//			Move input to console.
	//	Console can always be accessed through key X
	//	Esc returns to previous input mode


		}
	catch(const Glinde::ErrorMessage& msg)
		{
		fprintf(stderr,"%s\n",msg.messageGet());
		return -1;
		}

	return 0;
	}
