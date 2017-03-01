//@	{"targets":[{"name":"glinde","type":"application"}]}

#include "engine.hpp"
#include "errormessage.hpp"

int main()
	{
	try
		{
		Glinde::Engine engine;
		engine.run();
		}
	catch(const Glinde::ErrorMessage& msg)
		{
		fprintf(stderr,"%s\n",msg.messageGet());
		return -1;
		}

	return 0;
	}
