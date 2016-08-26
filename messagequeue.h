//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"messagequeue.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"messagequeue.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_MESSAGEQUEUE_H
#define GLINDE_MESSAGEQUEUE_H

#include "arraydynamicstl.h"
#include "mutex.h"
#include "message.h"
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
