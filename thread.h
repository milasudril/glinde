//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"thread.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"thread.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_THREAD_H
#define GLINDE_THREAD_H

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

	template<class Runner>
	class Thread:private ThreadBase
		{
		public:
			Thread(Runner&& runner):m_runner(std::move(runner))
				{start();}

			void run()
				{m_runner();}

			~Thread() noexcept
				{synchronize();}

		private:
			Runner m_runner;
		};
	}

#endif
