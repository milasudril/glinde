#ifdef __WAND__
target[name[stringkey.h] type[include]]
#endif

#ifndef GLINDE_STRINGKEY_H
#define GLINDE_STRINGKEY_H

#include <cstdint>

namespace Glinde
	{
	class Stringkey
		{
		public:
			explicit constexpr Stringkey(const char* str) noexcept:
				m_value(hash(str))
				{}

			constexpr bool operator<(const Stringkey& key) const noexcept
				{return m_value < key.m_value;}

			constexpr bool operator==(const Stringkey& key) const noexcept
				{return key.m_value==m_value;}

			constexpr bool operator!=(const Stringkey& key) const noexcept
				{return !(key==*this);}

		private:
			uint64_t m_value;

			static constexpr uint64_t OFFSET_BASIS=0xcbf29ce484222325;
			static constexpr uint64_t FNV_PRIME=0x100000001b3;
			static constexpr uint64_t hash(const char* str
				,uint64_t value_init=OFFSET_BASIS)
				{
				return *str=='\0'?
					 value_init
					:hash(str+1, (value_init^(uint64_t(*str)))*FNV_PRIME);
				}
		};
	}

#endif
