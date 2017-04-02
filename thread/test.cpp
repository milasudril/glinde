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
