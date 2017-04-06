//@	{"targets":[{"name":"messagequeue.hpp","type":"include"}]}

#ifndef GLINDE_MESSAGEQUEUE_HPP
#define GLINDE_MESSAGEQUEUE_HPP

#include "../arraydynamicstl.hpp"
#include "../thread/mutex.hpp"
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
			MessageQueue() noexcept:read_buffer(mb + 0),write_buffer(mb + 1)
				{}
		
			void post(double time_arrival,Message&& message)
				{
				Mutex::LockGuard guard(m_write_mutex);
				auto buffer=write_buffer;
				auto id=buffer->m_msg_id.get();
				buffer->m_queue.push(MessageHeader(time_arrival,id));
				if(id>=buffer->m_messages.length())
					{buffer->m_messages.append(std::move(message));}
				else
					{buffer->m_messages[id]=std::move(message);}
				}

			bool get(MessageHeader& header) noexcept
				{
				auto buffer=read_buffer;
				if(buffer->m_queue.size()==0)
					{return 0;}
				header=buffer->m_queue.top();
				buffer->m_queue.pop();
				return 1;
				}

			void process(MessageHeader& header,const Timeinfo& ti)
				{
				auto buffer=read_buffer;
				auto msg_id=static_cast<uint32_t>(header.id());
				assert(msg_id<buffer->m_messages.length());
				buffer->m_msg_id.release(msg_id);
				header.invalidate();
				auto msg=std::move( buffer->m_messages[msg_id] );
				msg.process(ti);
				}

			void swapBuffers()
				{
				Mutex::LockGuard guard(m_write_mutex);
				std::swap(read_buffer,write_buffer);
				}

		private:
			struct MessageBuffer
				{
				std::priority_queue< Message,ArrayDynamicSTL<MessageHeader>
					,std::greater<MessageHeader> > m_queue;		
				IdGenerator<uint32_t
					,std::priority_queue<uint32_t,ArrayDynamicSTL<uint32_t>,std::greater<uint32_t>  >
					> m_msg_id;
				ArrayDynamic<Message> m_messages;
				};

			MessageBuffer mb[2];
			Mutex m_write_mutex;
			MessageBuffer* read_buffer;
			MessageBuffer* write_buffer;
		};
	};

#endif
