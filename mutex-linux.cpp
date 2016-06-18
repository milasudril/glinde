#ifdef __WAND__
target[name[mutex.o] type[object] platform[;GNU/Linux] dependency[pthread;external]]
#endif

#include "mutex.h"
#include <pthread.h>

using namespace Glinde;

static pthread_mutex_t* mutex(uint8_t* bytes)
	{
	return reinterpret_cast<pthread_mutex_t*>(bytes);
	}

Mutex::Mutex()
	{
	static_assert(sizeof(m_impl)==sizeof(pthread_mutex_t),"Implementation size is wrong");
	pthread_mutex_init(mutex(m_impl.data),NULL);
	}

Mutex::~Mutex()
	{
	pthread_mutex_destroy(mutex(m_impl.data));
	}

void Mutex::lock() noexcept
	{
	pthread_mutex_lock(mutex(m_impl.data));
	}

void Mutex::unlock() noexcept
	{
	pthread_mutex_unlock(mutex(m_impl.data));
	}
