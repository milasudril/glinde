#ifdef __WAND__
target[name[logwriter.h] type[include]]
dependency[logwriter.o]
#endif

/**\file logwriter.h \brief Functions for writing log messages.
 */

#ifndef GLINIE_LOGWRITER_H
#define GLINIE_LOGWRITER_H

namespace Glinie
	{
#ifdef NDEBUG
	enum class LogMessageType:unsigned int{INFORMATION,WARNING,ERROR};
#else
	/**\brief Contains constants defining message types.
	 *
	 * This enumerator contains all possible message types.
	*/
	enum class LogMessageType:unsigned int
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
		 \note This message type is only availible in debug mode.
		 */
		};
#endif

	/**\brief Writes a message to the console (or the standard error stream).
	 *
	 * This function writes a message to the console.
	 *
	 * \param type is any value listed in LogMessageType
	 * \param format is a `printf`-style format string
	 *
	*/
	void logWrite(LogMessageType type,const char* format,...);

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
	 * message printed by the "deathtrap" handler should never be seen in a
	 * working program.
	 *
	*/
	void deathtrapHandlerActivate();
	};

#endif
