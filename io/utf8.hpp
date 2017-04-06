//@	{
//@	 "targets":[{"name":"utf8.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"utf8.o","rel":"implementation"}]
//@	}

#include "../string.hpp"
#include "../arraydynamic.hpp"
#include <cstdint>

namespace Glinde
	{
	static constexpr uint32_t UTF8_ACCEPT=0;
	static constexpr uint32_t UTF8_REJECT=1;

	uint32_t utf8_decode(uint32_t& state, uint32_t& codep, int8_t byte) noexcept;
	ArrayDynamic<uint32_t> utf8_decode(const char* str);
	String utf8_encode(Range<const uint32_t> src);
	}
