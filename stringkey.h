#ifdef __WAND__
target[name[stringkey.h] type[include]]
#endif

#ifndef GLINDA_STRINGKEY_H
#define GLINDA_STRINGKEY_H

#include "vectortype.h"
#include <cstdint>
#include <cstring>

namespace Glinda
	{
	class Stringkey
		{
		public:
			Stringkey(const char* str) noexcept
				{
				 strncpy(m_content.values,str,sizeof(m_content.values));
				}

			bool operator<(const Stringkey& key) const noexcept
				{
				return memcmp(&m_content.x,&key.m_content.x,sizeof(m_content.x))<0;
				}

		private:
			union
				{
				vec4_t<int32_t> x;
				char values[16];
				} m_content;
		};
	}

#endif
