#ifdef __WAND__
target[name[usereventhandler.o] type[object]]
#endif

#include "usereventhandler.h"
#include "worldobject.h"
#include "debug.h"

using namespace Glinda;

void UserEventHandler::onMouseMove(Window& source,double x,double y)
	{
	int w;
	int h;
	source.sizeGet(w,h);
	GLINDA_DEBUG_PRINT("Mouse move %.8g %.8g",x,y);
	r_camera->pitchSet(std::acos(-1.0f)*(1.0f - float(y/h) ))
		.yawSet(4.0f*std::acos(0.0f)*(float(x/w) - 1.0f/2.0f) )
		.headingUpdate();
	}
