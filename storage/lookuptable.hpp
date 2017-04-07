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

#include "new.hpp"
#include <map>
#include <memory>

namespace Glinde
	{
	/** \brief Look-up table with simple interface
	*/
	template<class Key,class Value>
	class LookupTable
		{
		public:
			Value* insert(Key&& key,Value&& val)
				{
				auto ip=m_data.insert(std::make_pair(key,std::make_unique<Value>(std::move(val))));
				if(ip.second)
					{return ip.first->second.get();}
				return nullptr;
				}

			Value* find(const Key& key) noexcept
				{
				auto i=m_data.find(key);
				if(i!=m_data.end())
					{return i->second.get();}
				return nullptr;
				}

			template<class ObjectCreator>
			Value& find(Key&& key,ObjectCreator&& create)
				{
				auto i=find(key);
				return i==nullptr?
					*insert(std::move(key),create())
					:*i;
				}

		private:
			std::map<Key,std::unique_ptr<Value>> m_data;
		};
	}

#endif // GLINDE_LOOKUPTABLE_HPP
