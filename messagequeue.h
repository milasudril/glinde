#ifdef __WAND__
target[name[messagequeue.h] type[include]]
dependency[messagequeue.o]
#endif

#ifndef GLINDE_MESSAGEQUEUE_H
#define GLINDE_MESSAGEQUEUE_H

#include "arraydynamicstl.h"
#include "mutex.h"
#include <queue>

namespace Glinde
	{
	class Message;

	class MessageQueue
		{
		public:
			MessageQueue();
			~MessageQueue();

			MessageQueue& post(Message&& message);
			MessageQueue& post(const Message& message);
			bool get(Message& message) noexcept;

		private:
			Mutex m_mutex;
			std::priority_queue< Message,ArrayDynamicSTL<Message> > m_queue;
		};
	};

#endif
