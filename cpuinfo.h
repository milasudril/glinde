#ifdef __WAND__
target[name[cpuinfo.h] type[include]]
#endif

#ifndef GLINDA_CPUINFO_H
#define GLINDA_CPUINFO_H

#include <cstdint>

namespace Glinda
	{
	namespace CPUInfo
		{
		static constexpr uint16_t word_test=0x00ff;
		inline bool constexpr bigEndianIs()
			{
			return ( *(reinterpret_cast<const uint8_t*>(&word_test))==0x0);
			}
		}
	}

#endif
