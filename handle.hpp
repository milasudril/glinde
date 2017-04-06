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
//@	{"targets":[{"name":"handle.pp","type":"include"}]}

#ifndef GLINDE_HANDLE_HPP
#define GLINDE_HANDLE_HPP

#include <type_traits>
#include <utility>

namespace Glinde
	{
	template<class T,T null_value,class Deleter>
	class Handle
		{
		public:
			static constexpr T null()
				{return null_value;}

			Handle():m_handle(null_value){}

			explicit Handle(T handle,const Deleter& deleter
				,typename std::enable_if< !std::is_reference<Deleter>::value >::type* dummy=nullptr)
				noexcept:
				m_handle(handle),m_d(deleter)
				{}

			explicit Handle(T handle,Deleter&& deleter
				,typename std::enable_if< !std::is_reference<Deleter>::value >::type* dummy=nullptr)
				noexcept:
				m_handle(handle),m_d(std::move(deleter))
				{}

			Handle(const Handle&)=delete;

			Handle(Handle&& h) noexcept:
				m_handle(h.handle),m_d(std::move(h.m_d))
				{h.m_handle=null();}

			~Handle()
				{
				if(m_handle!=null())
					{m_d(m_handle);}
				}

			Handle& operator=(Handle&& h) noexcept
				{
				std::swap(h.m_handle,m_handle);
				std::swap(h.m_d,m_d);
				return *this;
				}

			Handle& operator=(const Handle&)=delete;

			T get() const noexcept
				{return m_handle;}

			T operator->() const noexcept
				{return m_handle;}

			operator bool() const noexcept
				{return !(m_handle==null());}

			bool operator==(const Handle& other) const
				{return other.m_handle==m_handle;}

		private:
			T m_handle;
			Deleter m_d;
		};
	}

#endif
