//@	{
//@	 "targets":[{"name":"exceptionhandler.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"exceptionhandler.o","rel":"implementation"}]	
//@}

#ifndef ANGLE_EXCEPTIONHANDLER_HPP
#define ANGLE_EXCEPTIONHANDLER_HPP

#include "error.hpp"

namespace Angle
	{
	class ExceptionHandler
		{
		public:
			virtual void raise(const Error& err)
				{throw err;}
		};

	extern ExceptionHandler eh_default;
	extern ExceptionHandler* r_eh;
	}

#endif
