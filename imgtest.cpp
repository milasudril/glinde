#ifdef __WAND__
target[name[imgtest] type[application]]
#endif

#include "image.h"
#include "archive.h"
#include "logwriter.h"
#include "errormessage.h"

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
		logWrite(Log::MessageType::ERROR,"%s",message.messageGet());
		return -1;
		}
	return 0;
	}
