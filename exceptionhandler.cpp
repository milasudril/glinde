//@	{
//@	 "targets":[{"name":"exceptionhandler.o","type":"object"}]
//@	}

#include "exceptionhandler.hpp"

using namespace Angle;

ExceptionHandler Angle::eh_default;
ExceptionHandler* Angle::r_eh=&Angle::eh_default;
