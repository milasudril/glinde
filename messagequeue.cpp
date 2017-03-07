//@	{"targets":[{"name":"messagequeue.o","type":"object"}]}

#include "messagequeue.hpp"
#include "message.hpp"

using namespace Glinde;

bool MessageQueue::get(Message& message) noexcept
	{
	Mutex::LockGuard guard(m_mutex);
	if(m_queue.size()==0)
		{
		m_seq_next=0;
		return 0;
		}

	auto& msg=const_cast<Message&>( m_queue.top() );
	message=std::move(msg);
	m_queue.pop();
	return 1;
	}
