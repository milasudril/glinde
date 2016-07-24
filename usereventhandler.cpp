//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"usereventhandler.o",
//@	            "type":"object"
//@	        }
//@	    ]
//@	}

#include "usereventhandler.h"
#include "worldobject.h"
#include "worlddefault.h"
#include "debug.h"

using namespace Glinde;

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
		auto yaw=4.0f*std::acos(0.0f)*(float(x/w) - 1.0f/2.0f);
		auto pitch=std::acos(-1.0f)*(1.0f - float(y/h) );

	/*	auto& player=r_world->playerGet();
		player.eyesGet()
			.pitchSet(pitch)
			.yawSet(yaw )
			.headingUpdate();

		player.rotZSet(std::acos(-1.0f)-yaw);*/

		objectUpdate();
		}
	}

void UserEventHandler::objectUpdate()
	{
/*	auto& player=r_world->playerGet();

	auto F_0=player.dampingGet();

	glm::vec3 F={0,0,0};
	if(m_move_flags==0)
		{
		player.forceSet(F);
		return;
		}

	auto& heading=player.headingGet();

	if(m_move_flags&MOVE_FORWARD)
		{F+=glm::vec3(heading.x,heading.y,0.0f);}

	if(m_move_flags&MOVE_BACKWARD)
		{F-=glm::vec3(heading.x,heading.y,0.0f);}

	if(m_move_flags&STRAFE_LEFT)
		{F+=glm::vec3(-heading.y,heading.x,0.0f);}

	if(m_move_flags&STRAFE_RIGHT)
		{F-=glm::vec3(-heading.y,heading.x,0.0f);}

	F=F_0*normalize(F);

	player.forceSet(F);*/
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
		objectUpdate();
		}
	}

void UserEventHandler::onKeyUp(Window& source,uint8_t key)
	{
	auto flag=keyToFlag(key);
	if(flag!=MENU_MODE && !(m_move_flags&MENU_MODE))
		{
		m_move_flags&=~flag;
		objectUpdate();
		}
	}

void UserEventHandler::onMouseDown(Window& source,int button)
	{
/*	auto& player=r_world->playerGet();
	auto m=player.massGet();
	auto g=9.81f;
	auto t=0.25f;

	GLINDE_DEBUG_PRINT("Player clicked #0;",button);
	switch(button)
		{
		case 1:
			player.normalImpulseSet(glm::vec3(0.0f,0.0f,1.5f*m*g*t));
			break;
		}*/
	}
