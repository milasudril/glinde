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
//@	 "targets":
//@		[{
//@		 "name":"thread.o","type":"object"
//@		,"dependencies":[{"ref":"pthread","rel":"external"}]
//@		}]
//@	}

#include "thread.hpp"
#include <pthread.h>

using namespace Glinde;

static void* thread_entry(void* thread)
	{
	reinterpret_cast<ThreadBase*>(thread)->run();
	return nullptr;
	}

ThreadBase::ThreadBase()
	{
	static_assert(sizeof(m_handle)>=sizeof(pthread_t),"Handle type is too small");
	}

void ThreadBase::start()
	{
	pthread_create(reinterpret_cast<pthread_t*>(&m_handle)
		,NULL,thread_entry,this);
	}

ThreadBase::~ThreadBase() noexcept
	{}

void ThreadBase::synchronize() noexcept
	{pthread_join(m_handle,NULL);}