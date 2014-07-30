#ifdef __WAND__
target[name[init.h] type[include]]
dependency[init.o]
#endif

#include <herbs/exceptionmissing/exceptionmissing.h>

#include <GL/glew.h>

namespace Angle
	{
	void initException(GLenum status);

	inline void init()
		{
		glewExperimental=true;
		auto status=glewInit();
		if(status!=GLEW_OK)
			{initException(status);}
		}
	}
