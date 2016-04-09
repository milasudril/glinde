#ifdef __WAND__
target[name[stringkey.h] type[include]]
dependency[stringkey.o]
#endif

#ifndef GLINDA_STRINGKEY_H
#define GLINDA_STRINGKEY_H

#include <cstdint>

namespace Glinda
	{
	class Stringkey
		{
		public:
			Stringkey(const char* str) noexcept;

			bool operator<(const Stringkey& key) const noexcept
				{return m_value < key.m_value;}

		private:
			uint64_t m_value;
		};
	}

#endif
