//@	{"targets":[{"name":"messagequeue.hpp","type":"include"}]}

#ifndef GLINDE_MESSAGEQUEUE_HPP
#define GLINDE_MESSAGEQUEUE_HPP

#include "../arraydynamicstl.hpp"
#include "../mutex.hpp"
#include "../idgenerator.hpp"
#include "message.hpp"
#include "messageheader.hpp"
#include <queue>

namespace Glinde
	{
	class Timeinfo;

	class MessageQueue
		{
		public:
			void post(double time_arrival,Message&& message)
				{
				Mutex::LockGuard guard(m_mutex);
				auto id=m_msg_id.get();
				m_queue.push(MessageHeader(time_arrival,id));
				if(id>=m_messages.length())
					{m_messages.append(std::move(message));}
				else
					{m_messages[id]=std::move(message);}
				}

			bool get(MessageHeader& header) noexcept
				{
				if(m_queue.size()==0)
					{return 0;}
				header=m_queue.top();
				m_queue.pop();
				return 1;
				}

			void process(MessageHeader& header,const Timeinfo& ti) noexcept
				{
				auto msg_id=header.id();
				assert(msg_id<m_messages.length());
				m_msg_id.release(msg_id);
				header.invalidate();
				auto msg=std::move( m_messages[msg_id] );
				msg.process(ti);
				}

		private:
			Mutex m_mutex;
			std::priority_queue< Message,ArrayDynamicSTL<MessageHeader>
				,std::greater<MessageHeader> > m_queue;
		
			IdGenerator<uint32_t
				,std::priority_queue<uint32_t,ArrayDynamicSTL<uint32_t>,std::greater<uint32_t>  >
				> m_msg_id;
			ArrayDynamic<Message> m_messages;
		};
	};

#endif
