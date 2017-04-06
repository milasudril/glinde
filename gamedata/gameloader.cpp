/*
Glinde
Copyright (C) 2017  Torbjörn Rathsman

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/
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
			,Message{*r_eng,std::make_unique<Game>(*r_msgqueue,m_src.begin()),0});
		}
	catch(const ErrorMessage& msg)
		{
		m_error=msg;
		r_msgqueue->post(0,Message{*r_eng,this,0});
		}
	}