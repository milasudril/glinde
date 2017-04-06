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
//@	{"targets":[{"name":"log.hpp","type":"include"}]}

#ifndef GLINDE_LOG_HPP
#define GLINDE_LOG_HPP

#include <initializer_list>

namespace Glinde
	{
	class Variant;

	class Log
		{
		public:
			/**\brief Contains constants defining message types.
			*
			* This enumerator contains all possible message types.
			*/
			enum class MessageType:unsigned int
				{
				INFORMATION
				/**The message is purely informational and can be ignored.*/
				,WARNING
				/**The message contains a warning. A warning should be safe to ignore,
				* but the system may not work exactly as intended.*/

				,ERROR
				/**The message contains an error message. After an error, the current
				action did not proceed.*/

				,DEBUG
				/**The message is a debug message.
				*/
				,LINE_QUOTE
				/**Indented line useful for quoting program output
				*/
				};

			class Writer
				{
				public:
					virtual void write(MessageType type,const char* message) noexcept=0;
					virtual void progress(const char* message,double x) noexcept=0;
					virtual void progressInit() noexcept=0;
					virtual void progressEnd() noexcept=0;
				};

			/**\brief Writes a message to the console (or the standard error stream).
			*
			* This function writes a message to the console.
			*
			* \param type is any value listed in Log::MessageType
			*
			*/
			virtual void write(MessageType type,const char* format
				,const std::initializer_list<Variant>& args)=0;

			virtual void progressInit()=0;

			virtual void progressEnd()=0;

			virtual void progress(double x,const char* message)=0;
		};
	}

#endif
