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
//@	"targets":
//@		[{
//@		 "dependencies":[{"ref":"pthread","rel":"external"}]
//@		,"name":"mutex.o","type":"object"
//@	    }]
//@	}

#include "mutex.hpp"
#include <pthread.h>

using namespace Glinde;

static pthread_mutex_t* mutex(uint8_t* bytes)
	{
	return reinterpret_cast<pthread_mutex_t*>(bytes);
	}

Mutex::Mutex()
	{
	static_assert(sizeof(m_impl)==sizeof(pthread_mutex_t),"Implementation size is wrong");
	pthread_mutex_init(mutex(m_impl.data),NULL);
	}

Mutex::~Mutex()
	{
	pthread_mutex_destroy(mutex(m_impl.data));
	}

void Mutex::lock() noexcept
	{
	pthread_mutex_lock(mutex(m_impl.data));
	}

void Mutex::unlock() noexcept
	{
	pthread_mutex_unlock(mutex(m_impl.data));
	}
