//@	{
//@	 "dependencies_extra":[{"ref":"messagequeue.o","rel":"implementation"}]
//@	,"targets":[{"name":"messagequeue.hpp","type":"include"}]
//@	}
#ifndef GLINDE_MESSAGEQUEUE_HPP
#define GLINDE_MESSAGEQUEUE_HPP

#include "arraydynamicstl.hpp"
#include "mutex.hpp"
#include "message.hpp"
#include <queue>

namespace Glinde
	{
	class MessageQueue
		{
		public:
			MessageQueue():m_seq_next(0){}

			void post(Message&& message)
				{
				Mutex::LockGuard guard(m_mutex);
				message.seqSet( m_seq_next );
				++m_seq_next;
				m_queue.push(std::move(message));
				}

			bool get(Message& message) noexcept;

		private:
			Mutex m_mutex;
			std::priority_queue< Message,ArrayDynamicSTL<Message>
				,std::greater<Message> > m_queue;
			uint16_t m_seq_next;
		};
	};

#endif
