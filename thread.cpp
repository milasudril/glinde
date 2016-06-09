#ifdef __WAND__
target[name[thread.o] type[object] platform[;GNU/Linux] dependency[pthread;external]]
#endif

#include "thread.h"
#include <pthread.h>

using namespace Glinde;

static void* thread_entry(void* runner)
	{
	reinterpret_cast<Thread::Runner*>(runner)->run();
	return nullptr;
	}

Thread::Thread(Runner& runner)
	{
	static_assert(sizeof(m_handle)>=sizeof(pthread_t),"Handle type is too small");

	pthread_create(reinterpret_cast<pthread_t*>(&m_handle)
		,NULL,thread_entry,&runner);
	}

Thread::~Thread()
	{
	pthread_join(m_handle,NULL);
	}
