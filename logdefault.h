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
#include "string.h"

#ifndef GLINDE_LOGDEFAULT_H
#define GLINDE_LOGDEFAULT_H

namespace Glinde
	{
	class MessageQueue;
	class LogDefault:public Log
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

		private:
			class MessageData
				{
				public:
					MessageData():m_message(nullptr){}
					explicit MessageData(MessageType type,const char* text);
					~MessageData() noexcept;
					MessageData(const MessageData&)=delete;
					MessageData& operator=(const MessageData&)=delete;
					MessageData(MessageData&& msgdata) noexcept:
						m_message(msgdata.m_message),m_type(msgdata.m_type)
						{msgdata.m_message=nullptr;}
					MessageData& operator=(MessageData&& msgdata) noexcept
						{
						std::swap(msgdata.m_message,m_message);
						m_type=msgdata.m_type;
						return *this;
						}

					const char* textGet() const noexcept
						{return m_message;}

					MessageType typeGet() const noexcept
						{return m_type;}

				private:
					char* m_message;
					MessageType m_type;
				};

			class MessageCallback
				{
				public:
					MessageCallback(LogDefault& log):r_log(&log)
						{}

					void operator()(uint64_t time,const MessageData& logmessage);

				private:
					LogDefault* r_log;
				};

			Message::Processor<MessageData,MessageCallback> m_msg_proc;

			ArrayFixed<Writer*,4> m_writers;
			MessageQueue* r_queue;

			void write(MessageType type,const char* message);
		};
	}

#endif
