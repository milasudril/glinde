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
//@	"targets":[{"name":"messageheader.hpp","type":"include"}]
//@	}
#ifndef GLINDE_MESSAGEHEADER_HPP
#define GLINDE_MESSAGEHEADER_HPP

#include "../storage/vectortype.hpp"
#include <limits>

namespace Glinde
	{
	class MessageHeader
		{
		public:
			MessageHeader() noexcept:
				MessageHeader(0,std::numeric_limits<uint64_t>::max())
				{}

			explicit MessageHeader(double arrival_time,uint64_t id) noexcept
				{
				m_data.content.arrival_time=arrival_time;
				m_data.content.id=id;
				}

			double arrivalTime() const noexcept
				{return m_data.content.arrival_time;}

			uint64_t id() const noexcept
				{return m_data.content.id;}

			bool valid() const noexcept
				{return id()!=std::numeric_limits<uint64_t>::max();}

			void invalidate() noexcept
				{m_data.content.id=std::numeric_limits<uint64_t>::max();}

			bool operator>(MessageHeader msg) const noexcept
				{
				if(arrivalTime() > msg.arrivalTime())
					{return 1;}
				if(arrivalTime() == msg.arrivalTime()
					&& id() > msg.id())
					{return 1;}
				return 0;
				}

		private:
			union
				{
				vec2_t<uint64_t> vec;
				struct
					{
					double arrival_time;
					uint64_t id;
					} content;
				} m_data;
		};
	};

#endif
