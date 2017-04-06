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
//@	 "dependencies_extra":[{"ref":"logdefault.o","rel":"implementation"}]
//@	,"targets":[{"name":"logdefault.hpp","type":"include"}]
//@	}
#include "log.hpp"
#include "../storage/arrayfixed.hpp"
#include "../string.hpp"

#ifndef GLINDE_LOGDEFAULT_HPP
#define GLINDE_LOGDEFAULT_HPP

namespace Glinde
	{
	class MessageQueue;
	class Timeinfo;
	class LogDefault:public Log
		{
		private:
			struct ProgressInit{};
			struct ProgressEnd{};
		public:
			LogDefault() noexcept;

			void write(MessageType type,const char* format_string
				,const std::initializer_list<Variant>& args);

			void progressInit();

			void progressEnd();

			void progress(double x,const char* message);


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

			void operator()(const Timeinfo& ti,const String& str,Log::MessageType type) noexcept;

			void operator()(const Timeinfo& ti,const String& str,double x) noexcept;

			void operator()(const Timeinfo& ti,ProgressInit i,int) noexcept;

			void operator()(const Timeinfo& ti,ProgressEnd i,int) noexcept;

		private:
			ArrayFixed<Writer*,4> m_writers;
			MessageQueue* r_queue;

			void write(MessageType type,const char* message) noexcept;

			void progress(const char* message,double x) noexcept;

			void progress(ProgressInit) noexcept;

			void progress(ProgressEnd) noexcept;
		};
	}

#endif
