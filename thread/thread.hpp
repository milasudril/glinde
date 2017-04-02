//@	{
//@	 "targets":[{"name":"thread.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"thread.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_THREAD_HPP
#define GLINDE_THREAD_HPP

#include <cstdint>
#include <utility>

namespace Glinde
	{
	class ThreadBase
		{
		public:
			virtual void run()=0;

		protected:
			ThreadBase();
			virtual ~ThreadBase();

			void start();
			void synchronize() noexcept;

		private:
			intptr_t m_handle;
		};

	template<class Function>
	class Thread:private ThreadBase
		{
		public:
			Thread(Function&& entry):m_entry(std::move(entry))
				{start();}

			void run()
				{m_entry();}

			~Thread() noexcept
				{synchronize();}

		private:
			Function m_entry;
		};
	}

#endif
