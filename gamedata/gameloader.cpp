//@	{
//@	 "targets":[{"name":"gameloader.o","type":"object"}]
//@	}

#include "gameloader.hpp"
#include "../messagequeue/messagequeue.hpp"

using namespace Glinde;

GameLoader::GameLoader(GameLoader&& g) noexcept:
	 m_src(std::move(g.m_src)),r_msgqueue(g.r_msgqueue),r_eng(g.r_eng)
	,m_error(g.m_error)
	{}

void GameLoader::operator()()
	{
	try
		{
		r_msgqueue->post(0
			,Message{*r_eng,std::make_unique<Game>(*r_msgqueue,m_src.begin())});
		}
	catch(const ErrorMessage& msg)
		{
		m_error=msg;
		r_msgqueue->post(0,Message{*r_eng,this});
		}
	}