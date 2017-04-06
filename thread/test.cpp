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
//@	{"targets":[{"name":"threadtest","type":"application"}]}

#include "thread.hpp"
#include "../errormessage.hpp"
#include "../variant.hpp"

#include <cstdio>

using namespace Glinde;

void functionThatThrows()
	{
	throw ErrorMessage("Hello",{});
	}

class Test
    {
    public:
        void operator()()
            {
            try
                {
				functionThatThrows();
				}
            catch(const ErrorMessage& err)
                {fprintf(stderr,"%s\n",err.messageGet());}
            }
    };


int main()
    {
    Thread<Test>(Test{});
    return 0;
    }
