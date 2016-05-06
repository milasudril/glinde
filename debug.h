#ifdef __WAND__
target[name[debug.h] type[include]]
#endif

/**\file debug.h \brief Debugging macros
 *
 * This file contains macros for use when debugging.
 *
 */

#ifndef GLINIE_DEBUG_H
#define GLINIE_DEBUG_H

#ifdef NDEBUG
#include <cassert>
	#define GLINIE_DEBUG_PRINT(format,...)
	#define GLINIE_ASSERT_CALL(function,relation,value) (function)
#else

#include "logwriter.h"
#include <cassert>
#include <cstdarg>

	/**\brief Prints a debug message to the console
	 *
	 * This macro prints a debug message to the console. The macro follows
	 * the `printf` convention.
	*/
	#define GLINIE_DEBUG_PRINT(format,...)\
		Glinie::logWrite(Glinie::LogMessageType::DEBUG\
			,"%s:%u: " format,__FILE__,__LINE__,##__VA_ARGS__)

	/**\brief Function call with assertion on return value
	 *
	 * This macor is equivalent to `assert`, except that `function` is called
	 * also when `NDEBUG` is defined. When `NDEBUG` is not defined, the return
	 * value is checked against `value`, and if the `relation` becomes false,
	 * the process is aborted (as if regular `assert` was used).
	 *
	 * \warning Exiting through an assertion failure may leak system resources.
	 *
	*/
	#define GLINIE_ASSERT_CALL(function,relation,value) assert((function) relation (value))
#endif

#endif
