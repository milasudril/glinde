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
//@	 "targets":[{"name":"thread.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"thread.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_THREAD_HPP
#define GLINDE_THREAD_HPP

#include <cstdint>
#include <utility>

namespace Glinde
	{
	class ThreadBase
		{
		public:
			ThreadBase(const ThreadBase&)=delete;
			ThreadBase& operator=(const ThreadBase&)=delete;

			virtual void run()=0;

		protected:
			ThreadBase();
			virtual ~ThreadBase();

			void start();
			void synchronize() noexcept;

		private:
			intptr_t m_handle;
		};

	template<class Function>
	class Thread final:private ThreadBase
		{
		public:
			explicit Thread(Function&& entry):m_entry(std::move(entry))
				{start();}

			void run()
				{m_entry();}

			~Thread() noexcept
				{synchronize();}

		private:
			Function m_entry;
		};
	}

#endif
