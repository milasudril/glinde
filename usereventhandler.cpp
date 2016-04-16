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

	r_player->eyesGet().pitchSet(std::acos(-1.0f)*(1.0f - float(y/h) ))
		.yawSet(4.0f*std::acos(0.0f)*(float(x/w) - 1.0f/2.0f) )
		.headingUpdate();
	}

unsigned int UserEventHandler::keyToFlag(uint8_t key)
	{
	switch(key)
		{
		case 111:
			return MOVE_FORWARD;

		case 116:
			return MOVE_BACKWARD;

		case 113:
			return STRAFE_LEFT;

		case 114:
			return STRAFE_RIGHT;
		}
	return 0;
	}

void UserEventHandler::onKeyDown(Window& source,uint8_t key)
	{
//	GLINDA_DEBUG_PRINT("Got key event %u",key);
	auto& heading=r_player->headingGet();
	auto position=r_player->positionGet();
	float speed=1.0f;

	m_move_flags|=keyToFlag(key);

	if(m_move_flags&MOVE_FORWARD)
		{position+=static_cast<float>(m_dt)*speed*heading;}

	if(m_move_flags&MOVE_BACKWARD)
		{position-=static_cast<float>(m_dt)*speed*heading;}

	if(m_move_flags&STRAFE_LEFT)
		{
		position+=static_cast<float>(m_dt)*speed
				*glm::vec3{-heading.y,heading.x,heading.z};
		}

	if(m_move_flags&STRAFE_RIGHT)
		{
		position+=static_cast<float>(m_dt)*speed
				*glm::vec3{heading.y,-heading.x,heading.z};
		}

	r_player->positionSet(position);
	}

void UserEventHandler::onKeyUp(Window& source,uint8_t key)
	{
	m_move_flags&=~keyToFlag(key);
	}
