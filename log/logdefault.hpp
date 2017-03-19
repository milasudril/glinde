//@	{
//@	 "dependencies_extra":[{"ref":"logdefault.o","rel":"implementation"}]
//@	,"targets":[{"name":"logdefault.hpp","type":"include"}]
//@	}
#include "log.hpp"
#include "../arrayfixed.hpp"
#include "../string.hpp"
#include "../messagequeue/message.hpp"

#ifndef GLINDE_LOGDEFAULT_HPP
#define GLINDE_LOGDEFAULT_HPP

namespace Glinde
	{
	class MessageQueue;
	class LogDefault:public Log
		{
		private:
			class MessageData;
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

			void operator()(const MessageData& logmessage) noexcept
				{write(logmessage.type(),logmessage.text());}

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

					const char* text() const noexcept
						{return m_message;}

					MessageType type() const noexcept
						{return m_type;}

				private:
					char* m_message;
					MessageType m_type;
				};

			ArrayFixed<Writer*,4> m_writers;
			MessageQueue* r_queue;

			void write(MessageType type,const char* message);
		};
	}

#endif
