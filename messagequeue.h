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
				message.seqSet( seqGet() );
				m_queue.push(std::move(message));
				}

			bool get(Message& message) noexcept;

		private:
			Mutex m_mutex;
			std::priority_queue< Message,ArrayDynamicSTL<Message> > m_queue;
			ArrayDynamic<uint16_t> m_seq;
			uint16_t m_seq_next;
			uint16_t seqGet()
				{
				if(m_seq.length()==0)
					{
					auto ret=m_seq_next;
					++m_seq_next;
					return ret;
					}
				auto ret=*(m_seq.end() - 1);
				m_seq.truncate();
				return ret;
				}
		};
	};

#endif
