//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"log.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_LOG_H
#define GLINDE_LOG_H

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
				};

			/**\brief Writes a message to the console (or the standard error stream).
			*
			* This function writes a message to the console.
			*
			* \param type is any value listed in Log::MessageType
			*
			*/
			virtual void write(MessageType type,const char* format
				,const std::initializer_list<Variant>& args) noexcept=0;
		};
	}

#endif
