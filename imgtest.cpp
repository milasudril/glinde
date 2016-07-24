//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"imgtest",
//@	            "type":"application"
//@	        }
//@	    ]
//@	}
#include "image.h"
#include "archive.h"
#include "logwriter.h"
#include "errormessage.h"
#include "variant.h"

using namespace Glinde;

int main()
	{
	deathtrapHandlerActivate();
	try
		{
		Archive test("test.zip");
		Image testimg{Archive::File(test,"diffuse.png"),0};
		}
	catch(const ErrorMessage& message)
		{
		logWrite(Log::MessageType::ERROR,"#0;",{message.messageGet()});
		return -1;
		}
	return 0;
	}
