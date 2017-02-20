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
		virtual void raise(const Error& err)
			{throw err;}

		friend void exceptionRaise(const Error& err);
		};

	ExceptionHandler& exceptionHandlerSet(ExceptionHandler* eh) noexcept;
	void exceptionRaise(const Error& err);

	class ExceptionContext
		{
		public:
			explicit ExceptionContext(ExceptionHandler* eh) noexcept:
				r_handler_old(exceptionHandlerSet(eh))
				{}
		
			~ExceptionContext() noexcept
				{exceptionHandlerSet(&r_handler_old);}

		private:
			ExceptionHandler& r_handler_old;
		};
	}

#endif
