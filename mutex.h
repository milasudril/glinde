#ifdef __WAND__
target[name[mutex.h] type[include]]
dependency[mutex.o]
#endif

#ifndef GLINDE_MUTEX_H
#define GLINDE_MUTEX_H

#include <cstdint>

namespace Glinde
	{
	class Mutex
		{
		public:
			class LockGuard
				{
				public:
					LockGuard(Mutex& m):r_m(m)
						{m.lock();}

					~LockGuard()
						{r_m.unlock();}
				private:
					Mutex& r_m;
				};

			Mutex();
			~Mutex();

			void lock() noexcept;
			void unlock() noexcept;

		private:
#if (__amd64 || __x86_64 || __x86_64__ || __amd64__)
			static constexpr int MUTEX_SIZE=40;
#else
			static constexpr int MUTEX_SIZE=24;
#endif
			union
				{
				double dummy[MUTEX_SIZE/sizeof(double)];
				uint8_t data[MUTEX_SIZE];
				} m_impl;
		};
	}

#endif
