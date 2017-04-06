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
//@	{"targets":[{"name":"idgenerator.hpp","type":"include"}]}

#ifndef GLINDE_IDGENERATOR_HPP
#define GLINDE_IDGENERATOR_HPP

#include "storage/arraydynamicstl.hpp"
#include <stack>

namespace Glinde
	{
	template<class IdType,class Freelist=std::stack<IdType,ArrayDynamicSTL<IdType> > >
	class IdGenerator
		{
		public:
			IdGenerator() noexcept:m_id_next(-1){}

			IdType get() noexcept
				{
				if(m_freelist.size()==0)
					{
					++m_id_next;
					return m_id_next;
					}
				auto ret=m_freelist.top();
				m_freelist.pop();
				return ret;
				}

			IdGenerator& release(const IdType& id)
				{
				m_freelist.push(id);
				return *this;
				}

			IdGenerator& reset() noexcept
				{
				m_freelist.clear();
				m_id_next=-1;
				return *this;
				}

		private:
			Freelist m_freelist;
			IdType m_id_next;
		};
	}

#endif
