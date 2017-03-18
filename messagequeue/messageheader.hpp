//@	{
//@	"targets":[{"name":"messagequeue.hpp","type":"include"}]
//@	}
#ifndef GLINDE_MESSAGEHEADER_HPP
#define GLINDE_MESSAGEHEADER_HPP

#include "../vectortype.hpp"

namespace Glinde
	{
	class MessageHeader
		{
		public:
			MessageHeader(double arrival_time,uint64_t id) noexcept
				{
				m_data.content.arrival_time=arrival_time;
				m_data.content.id=id;
				}

			double arrivalTime() const noexcept
				{return m_data.content.arrival_time;}

			uint64_t id() const noexcept
				{return m_data.content.id;}

			bool operator>(MessageHeader msg) const noexcept
				{
				if(arrivalTime() > msg.arrivalTime())
					{return 1;}
				if(arrivalTime() == msg.arrivalTime())
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
			}
		};
	};

#endif
