//@	{
//@	 "targets":[{"name":"utf8.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"utf8.o","rel":"implementation"}]
//@	}

#include "../string.hpp"
#include "../arraydynamic.hpp"
#include <cstdint>

namespace Glinde
	{
	uint32_t utf8_decode(uint32_t& state, uint32_t& codep, int8_t byte) noexcept;
	ArrayDynamic<uint32_t> utf8_decode(const char* str);
	String utf8_encode(Range<const uint32_t> src);
	}
