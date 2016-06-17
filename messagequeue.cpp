#ifdef __WAND__
target[name[messagequeue.o] type[object]]
#endif

#include "messagequeue.h"
#include "message.h"

using namespace Glinde;

bool MessageQueue::get(Message& message) noexcept
	{
	Mutex::LockGuard guard(m_mutex);
	if(m_queue.size()==0)
		{return 0;}

	auto& msg=const_cast<Message&>( m_queue.top() );
	message=std::move(msg);
	m_queue.pop();
	m_seq.append(message.seqGet());
	return 1;
	}
