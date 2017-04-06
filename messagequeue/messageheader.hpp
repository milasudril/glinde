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
