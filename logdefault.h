//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"logdefault.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"logdefault.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#include "log.h"
#include "arrayfixed.h"
#include "message.h"

#ifndef GLINDE_LOGDEFAULT_H
#define GLINDE_LOGDEFAULT_H

namespace Glinde
	{
	class MessageQueue;
	class LogDefault:public Log,public Message::Processor
		{
		public:
			LogDefault() noexcept;

			void write(MessageType type,const char* format_string
				,const std::initializer_list<Variant>& args) noexcept;

			/**\brief Attaches an additional LogWriter to the log writing system.
			 *
			 * \return The selected index for the writer. If this is not possible
			 * the function returns (unsigned int)(-1).
			 *
			*/
			unsigned int writerAttach(Writer& writer) noexcept;

			/**\brief Attaches an additional log writer to the log writing system.
			*/
			void writerDetach(unsigned int index) noexcept;


			/**\brief Registers a "deathtrap" handler, that is called by the operating
			* system when the program behaves badley.
			*
			* This function registers a "deathtrap" handler, that is called by the
			* operating system when the program behaves badley. When a program error
			* is detected, an error message containing the current instruction pointer
			* is written to the standard error stream. After that, the program
			* terminates without saving any data.
			*
			* \note This function exists as a debugging aid, and ideally, the error
			* message printed by the "deathtrap" handler should never be seen in the
			* output if a working application.
			*/
			void deathtrapHandlerActivate();

			unsigned int queueAttach(MessageQueue& queue) noexcept
				{
				r_queue=&queue;
				return 0;
				}

			void queueDetach(unsigned int index) noexcept
				{r_queue=nullptr;}

			void operator()(const Message& message);

		private:
			ArrayFixed<Writer*,4> m_writers;
			MessageQueue* r_queue;

			void write(MessageType type,const char* message);
		};
	}

#endif
