#ifdef __WAND__
target[name[usereventhandler.o] type[object]]
#endif


#include "usereventhandler.h"
#include "worldobject.h"
#include "world.h"
#include "debug.h"

using namespace Glinda;

void UserEventHandler::onMouseMove(Window& source,double x,double y)
	{
	int w;
	int h;
	source.sizeGet(w,h);
	if(m_move_flags&MENU_MODE)
		{
	//	TODO: Do menu stuff
		}
	else
		{
		auto& player=r_world->playerGet();
		player.eyesGet().pitchSet(std::acos(-1.0f)*(1.0f - float(y/h) ))
			.yawSet(4.0f*std::acos(0.0f)*(float(x/w) - 1.0f/2.0f) )
			.headingUpdate();

		velocityUpdate();
		}
	}

void UserEventHandler::velocityUpdate()
	{
	auto& player=r_world->playerGet();

	glm::vec3 v={0,0,0};
	if(m_move_flags==0)
		{
	//	Stop player
	//	TODO: add momentum
		player.velocitySet(v);
		return;
		}

	auto& heading=player.headingGet();
	float speed=1.0f;

	if(m_move_flags&MOVE_FORWARD)
		{v+=heading;}

	if(m_move_flags&MOVE_BACKWARD)
		{v-=heading;}

	if(m_move_flags&STRAFE_LEFT)
		{v+=glm::vec3{-heading.y,heading.x,0.0f};}

	if(m_move_flags&STRAFE_RIGHT)
		{v-=glm::vec3{-heading.y,heading.x,0.0f};}

	v=speed*normalize(v);
	player.velocitySet(v);
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

		case 9:
			return MENU_MODE;
		}
	return 0;
	}

void UserEventHandler::onKeyDown(Window& source,uint8_t key)
	{
	auto flag=keyToFlag(key);
	if(flag==MENU_MODE || (m_move_flags&MENU_MODE))
		{
		if((m_move_flags&MENU_MODE) && flag==MENU_MODE )
			{
			source.cursorHideAndGrab();
			m_move_flags=0;
			}
		else
			{
			source.cursorShow();
			m_move_flags=MENU_MODE;
			}
		}
	else
		{
		switch(flag) //	Disable inconsistent flags
			{
			case STRAFE_LEFT:
				m_move_flags&=~STRAFE_RIGHT;
				break;

			case STRAFE_RIGHT:
				m_move_flags&=~STRAFE_LEFT;
				break;

			case MOVE_FORWARD:
				m_move_flags&=~MOVE_BACKWARD;
				break;

			case MOVE_BACKWARD:
				m_move_flags&=~MOVE_FORWARD;
				break;

			default:
				break;
			}

		m_move_flags|=flag;
		velocityUpdate();
		}
	}

void UserEventHandler::onKeyUp(Window& source,uint8_t key)
	{
	auto flag=keyToFlag(key);
	if(flag!=MENU_MODE && !(m_move_flags&MENU_MODE))
		{
		m_move_flags&=~flag;
		velocityUpdate();
		}
	}
