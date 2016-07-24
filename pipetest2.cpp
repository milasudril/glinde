//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"pipetest",
//@	            "type":"application"
//@	        }
//@	    ]
//@	}
#include "pipein.h"
#include "readbuffer.h"
#include "errormessage.h"

int main()
	{
	try
		{
		Glinde::PipeIn cxxpipe(Glinde::PipeIn::Stream::STDOUT,"g++",{"--version"});
	/*	Glinde::ReadBuffer buffer(cxxpipe);
		printf(">   ");
		while(!buffer.eof())
			{
			auto ch_in=buffer.byteRead();
			putchar(ch_in);
			if(ch_in=='\n' && !buffer.eof())
				{
				printf(">   ");
				}
			}*/

		printf("Status code: %u\n",cxxpipe.exitStatusGet());

		}
	catch(const Glinde::ErrorMessage& e)
		{
		printf("Error: %s\n",e.messageGet());
		}
	}
