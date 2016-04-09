#ifdef __WAND__
target[name[usereventhandler.o] type[object]]
#endif

#include "usereventhandler.h"
#include "worldobject.h"

using namespace Glinda;

void UserEventHandler::onMouseMove(Window& source,double x,double y)
	{
	int w;
	int h;
	source.sizeGet(w,h);
	r_camera->pitchSet(std::acos(-1)*y/h)
		.yawSet(4.0f*std::acos(0)*(x-w/2)/w)
		.headingUpdate();
	}
