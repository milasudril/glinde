//@	{
//@	 "dependencies_extra":[{"ref":"messagequeue.o","rel":"implementation"}]
//@	,"targets":[{"name":"messagequeue.hpp","type":"include"}]
//@	}
#ifndef GLINDE_MESSAGEQUEUE_HPP
#define GLINDE_MESSAGEQUEUE_HPP

#include "../arraydynamicstl.hpp"
#include "../mutex.hpp"
#include "../idgenerator.hpp"
#include "message.hpp"
#include <queue>

namespace Glinde
	{
	class MessageQueue
		{
		public:
			void post(double time_arrival,Message&& message)
				{
				Mutex::LockGuard guard(m_mutex);
				auto id=m_msg_id.idGet();
				m_queue.push(MessageHeader(time_arrival,id));
				if(id>=m_messages.size())
					{m_messages.append(std::move(message));}
				else
					{m_messages[id]=std::move(message);}
				}

			bool pop(MessageHeader& header) noexcept
				{
				if(m_queue.size()==0)
					{return 0;}
				header=m_queue.top();
				m_queue.pop();
				return 1;
				}

			void process(uint32_t msg_id) noexcept
				{	
				assert(msg_id<m_messages.size());
				auto msg=std::move( m_messages[msg_id] );
				msg.process();
				m_msg_id.release(msg_id);
				}

		private:
			Mutex m_mutex;
			std::priority_queue< Message,ArrayDynamicSTL<MessageHeader>
				,std::greater<MessageHeader> > m_queue;
		
			IdGenerator<uint32_t> m_msg_id;
			ArrayDynamic<Message> m_messages;
		};
	};

#endif
