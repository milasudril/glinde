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
//@	 "dependencies_extra":[{"ref":"mutex.o","rel":"implementation"}]
//@	,"targets":[{"name":"mutex.h","type":"include"}]
//@	}

#ifndef GLINDE_MUTEX_HPP
#define GLINDE_MUTEX_HPP

#include <cstdint>

namespace Glinde
	{
	class Mutex
		{
		public:
			class LockGuard
				{
				public:
					LockGuard(const LockGuard&)=delete;
					LockGuard& operator=(const LockGuard&)=delete;
					explicit LockGuard(Mutex& m):r_m(m)
						{m.lock();}

					~LockGuard()
						{r_m.unlock();}
				private:
					Mutex& r_m;
				};

			Mutex();
			~Mutex();

			void lock() noexcept;
			void unlock() noexcept;

		private:
#if (__amd64 || __x86_64 || __x86_64__ || __amd64__)
			static constexpr int MUTEX_SIZE=40;
#else
			static constexpr int MUTEX_SIZE=24;
#endif
			union
				{
				double dummy[MUTEX_SIZE/sizeof(double)];
				uint8_t data[MUTEX_SIZE];
				} m_impl;
		};
	}

#endif
