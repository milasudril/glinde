#ifdef __WAND__
target[name[messagequeue.o] type[object]]
#endif

#include "messagequeue.h"
#include "message.h"

using namespace Glinde;

MessageQueue::MessageQueue(){}
MessageQueue::~MessageQueue(){}

MessageQueue& MessageQueue::post(Message&& message)
	{
	Mutex::LockGuard guard(m_mutex);
	m_queue.push(std::move(message));
	return *this;
	}

MessageQueue& MessageQueue::post(const Message& message)
	{
	Mutex::LockGuard guard(m_mutex);
	m_queue.push(message);
	return *this;
	}

bool MessageQueue::get(Message& message) noexcept
	{
	Mutex::LockGuard guard(m_mutex);
	if(m_queue.size()==0)
		{return 0;}

	auto& msg=const_cast<Message&>( m_queue.top() );
	message=std::move(msg);
	m_queue.pop();
	return 1;
	}
