#ifdef __WAND__
target[name[errormessage.h] type[include]]
dependency[errormessage.o]
#endif

#ifndef ERRORMESSAGE_H
#define ERRORMESSAGE_H

/**\file errormessage.h \brief Defines an error message.
 */

#include <cstddef>

namespace Glinie
	{
	/**\brief Class representing a formated error message.
	 *
	 * The exception handling within Glinie is based on throwing ErrorMessage
	 * objects. An ErrorMessage contains a formated string describing the
	 * exception in a user-friendly way. Typically, the message is formated at
	 * the point where the exception is thrown. The message can then be catched
	 * and displayed to the user.
	 *
	 */
	class ErrorMessage
		{
		public:
			/**\brief Initializes an ErrorMessage.
			 *
			 * This constructor initializes the ErrorMessage by following
			 * `printf`-style conventions.
			 *
			 * \note If the message does not fit within the internal buffer, it
			 * is truncated, but always zero terminated. However, this should
			 * not be a problem, since it is not useful with to long messages
			 * anyways.
			 *
			*/
			explicit ErrorMessage(const char* format,...) noexcept;


			/**\brief Retrievs a pointer to the error message.
			 *
			 * This function returns a pointer to the error message.
			*/
			const char* messageGet() const noexcept
				{return m_buffer;}

		private:
			static constexpr size_t SIZE_BUFFER=1024;
			char m_buffer[SIZE_BUFFER];
		};
	}

#endif
