/*
Glinde
Copyright (C) 2017  Torbjörn Rathsman

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/
//@	{
//@	 "dependencies_extra":[{"ref":"debug.o","rel":"implementation"}]
//@	,"targets":[{"name":"debug.hpp","type":"include"}]
//@	}
/**\file debug.hpp \brief Debugging macros
 *
 * This file contains macros for use when debugging.
 *
 */

#ifndef GLINDE_DEBUG_HPP
#define GLINDE_DEBUG_HPP

#ifdef NDEBUG
#include <cassert>
	#define GLINDE_DEBUG_PRINT(format,...)
	#define GLINDE_ASSERT_CALL(function,relation,value) (function)
#else

#include <cassert>
#include <cstdarg>
#include <cstdint>
#include <initializer_list>

	/**\brief Prints a debug message to the console
	 *
	 * This macro prints a debug message to the console. The format string
	 * follows the syntax of format.
	*/
	#define GLINDE_DEBUG_PRINT(format,...)\
		Glinde::logWriteDebug(__FILE__,__LINE__,format,{__VA_ARGS__})

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
	#define GLINDE_ASSERT_CALL(function,relation,value) assert((function) relation (value))

namespace Glinde
	{
	class Variant;
	void logWriteDebug(const char* file,uint64_t line,const char* format
		,const std::initializer_list<Variant>& args) noexcept;
	}

#endif

#endif
