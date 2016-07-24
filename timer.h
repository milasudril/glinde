//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"timer.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_TIMER_H
#define GLINDE_TIMER_H

namespace Glinde
	{
	class Timer
		{
		public:
			virtual ~Timer()=default;
			virtual void wait() const noexcept=0;
			virtual double delayGet() const noexcept=0;
		};
	}

#endif
