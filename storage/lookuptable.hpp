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
//@	 "targets":[{"name":"lookuptable.hpp","type":"include"}]
//@	}
#ifndef GLINDE_LOOKUPTABLE_HPP
#define GLINDE_LOOKUPTABLE_HPP

#include "arraydynamic.hpp"
#include <utility>
#include <algorithm>
#include <memory>

namespace Glinde
	{
	/** \brief Look-up table optimized for frequent lookup
	*/
	template<class Key,class Value
		,class Compare=std::less<Key> >
	class LookupTable
		{
		public:
			LookupTable():m_cmp(Compare{}),m_dirty(0){}

			LookupTable& insert(Key&& key,Value&& val) noexcept
				{
				m_data.append({std::move(key),make_unique(val)});
				m_dirty=1;
				return *this;
				}

			auto find(const Key& k) noexcept
				{
				auto first=m_data.begin();
				auto last=m_data.end();
				if(m_dirty)
					{
					std::sort(first,last,m_cmp);
					m_dirty=0;
					}
				auto i=std::lower_bound(first,last,m_cmp);
				if(i!=last && !m_cmp(k,i->first))
					{return i;}
				return last;
				}

		private:
			ArrayDynamic< std::pair<Key,std::unique_ptr<Value*> > > m_data;
			Compare m_cmp;
			mutable bool m_dirty;
		};
	}

#endif // GLINDE_LOOKUPTABLE_HPP
