//@	{
//@	 "targets":[{"name":"exceptionhandler.o","type":"object"}]
//@	}

#include "exceptionhandler.hpp"

using namespace Angle;

static ExceptionHandler eh_default;
static ExceptionHandler* r_eh=&eh_default;

ExceptionHandler& Angle::exceptionHandlerSet(ExceptionHandler* eh) noexcept
	{
	auto ret=r_eh;
	r_eh=eh==nullptr?&eh_default:eh;
	return *ret;
	}

void Angle::exceptionRaise(const Error& err)
	{
	r_eh->raise(err);
	}
