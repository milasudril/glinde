#ifdef __WAND__
target[name[debug.h] type[include]]
#endif

#ifndef GLINDA_DEBUG_H
#define GLINDA_DEBUG_H

#include <cassert>

#ifdef NDEBUG
	#define GLINDA_DEBUG_PRINT(format,...)
	#define GLINDA_ASSERT_CALL(function,relation,value) (function)
#else

#include "logwriter.h"
#include <cstdarg>
	#define GLINDA_DEBUG_PRINT(format,...)\
		Glinda::logWrite(Glinda::LogMessageType::DEBUG\
			,"%s:%u: " format,__FILE__,__LINE__,##__VA_ARGS__)

	#define GLINDA_ASSERT_CALL(function,relation,value) assert((function) relation (value))
#endif

#endif
