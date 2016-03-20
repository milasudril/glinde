#ifdef __WAND__
target[name[debug.h] type[include]]
#endif

#ifndef GLINDA_DEBUG_H
#define GLINDA_DEBUG_H

#ifdef NDEBUG
	#define GLINDA_DEBUG_PRINT(x)
	#define GLINDA_ASSERT_CALL(function,relation,value) (function)
#else

#include "logwriter.h"
#include <cassert>
#include <cstdarg>
	#define GLINDA_DEBUG_PRINT(format,...)\
		Glinda::logWrite(Glinda::LogMessageType::DEBUG\
			,"%s:%u: " format,__FILE__,__LINE__,##__VA_ARGS__)

	#define GLINDA_ASSERT_CALL(function,relation,value) assert((function) relation (value))
#endif

#endif
