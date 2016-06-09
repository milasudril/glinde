#ifdef __WAND__
target[name[thread.h] type[include]]
dependency[thread.o]
#endif

#ifndef GLINDE_THREAD_H
#define GLINDE_THREAD_H

#include <cstdint>

namespace Glinde
	{
	class Thread
		{
		public:
			class Runner
				{
				public:
					virtual ~Runner()=default;
					virtual void run() noexcept=0;

				};

			Thread(Runner& runner);
			~Thread();

		private:
			intptr_t m_handle;
		};
	}

#endif
