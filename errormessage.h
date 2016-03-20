#ifdef __WAND__
target[name[errormessage.h] type[include]]
dependency[errormessage.o]
#endif

#ifndef ERRORMESSAGE_H
#define ERRORMESSAGE_H

#include <cstddef>

namespace Glinda
	{
	class ErrorMessage
		{
		public:
			explicit ErrorMessage(const char* format,...) noexcept;

			const char* messageGet() const noexcept
				{return m_buffer;}

		private:
			static constexpr size_t SIZE_BUFFER=1024;
			char m_buffer[SIZE_BUFFER];
		};
	}

#endif
