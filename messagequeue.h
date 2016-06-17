#ifdef __WAND__
target[name[messagequeue.h] type[include]]
dependency[messagequeue.o]
#endif

#ifndef GLINDE_MESSAGEQUEUE_H
#define GLINDE_MESSAGEQUEUE_H

#include "arraydynamicstl.h"
#include "mutex.h"
#include "message.h"
#include <queue>

namespace Glinde
	{
	class Message;

	class MessageQueue
		{
		public:
			MessageQueue():m_seq_next(0){}
			~MessageQueue();

			template<class T>
			void post(uint64_t time,Message::Handler handler
				,const Range<const T>& data
				,typename std::enable_if< std::is_trivially_copyable<T>::value >::type* dummy=nullptr)
				{
				Mutex::LockGuard guard(m_mutex);
				auto seq=seqGet();
				m_queue.push(Message{time,seq,handler,data});
				}

			template<class T,size_t N>
			void post(uint64_t time,Message::Handler handler,void* object
				,const ArrayFixed<T,N>& data)
				{
				Mutex::LockGuard guard(m_mutex);
				auto seq=seqGet();
				m_queue.push(Message{time,seq,handler,object,data});
				}

			bool get(Message& message) noexcept;

		private:
			void post(Message&& msg);
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
