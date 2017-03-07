//@	{"targets":[{"name":"messagequeue.o","type":"object"}]}

#include "messagequeue.hpp"
#include "message.hpp"
#include <algorithm>

using namespace Glinde;

bool MessageQueue::get(Message& message) noexcept
	{
	Mutex::LockGuard guard(m_mutex);
	if(m_queue.size()==0)
		{return 0;}

	auto& msg=const_cast<Message&>( m_queue.top() );
	message=std::move(msg);
	m_queue.pop();
	m_seq_next=std::min(message.seq(),m_seq_next);
	return 1;
	}
