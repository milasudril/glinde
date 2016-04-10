#ifdef __WAND__
target[name[timer.h] type[include]]
dependency[timer.o]
#endif

#ifndef GLINDA_TIMER_H
#define GLINDA_TIMER_H

#include <cstdint>

namespace Glinda
	{
	class Timer
		{
		public:
			Timer(double frequency);
			~Timer();
			void wait() noexcept;


		private:
			struct Impl;
			Impl* m_impl;
		};
	}

#endif
