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
//@	{"targets":[{"name":"instancecounter.hpp","type":"include"}]}

#ifndef GLINDE_INSTANCECOUNTER_HPP
#define GLINDE_INSTANCECOUNTER_HPP

#include <cassert>
#include <cstddef>
#include <utility>

namespace Glinde
	{
	template<class T>
	class InstanceCounter
		{
		public:
			InstanceCounter(const InstanceCounter&)=delete;
			InstanceCounter(InstanceCounter&&)=delete;
			InstanceCounter& operator=(const InstanceCounter&)=delete;
			InstanceCounter& operator=(InstanceCounter&&)=delete;

			InstanceCounter() noexcept:m_obj(nullptr),m_counter(0){}

			template<class U,class... Args>
			T& get(Args&&... args)
				{
				if(m_counter==0)
					{m_obj=new T(std::forward<Args>(args)...);}
				++m_counter;
				return *m_obj;
				}

			T& get() noexcept
				{
				assert(m_counter!=0);
				return *m_obj;
				}

			void release() noexcept
				{
				--m_counter;
				if(m_counter==0)
					{delete m_obj;}
				}

		private:
			T* m_obj;
			size_t m_counter;
		};
	}

#endif