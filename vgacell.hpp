//@	{
//@	 "targets":[{"name":"vgacell.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"vgacell.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_VGACELL_HPP
#define GLINDE_VGACELL_HPP

#include <cstdint>

namespace Glinde
	{
	class VGACell
		{
		public:
			constexpr VGACell(uint8_t ch,uint8_t mask) noexcept:
				m_ch(ch),m_mask(mask)
				{}

			uint32_t codepoint() const noexcept;

			constexpr uint8_t colorMask() const noexcept
				{return m_mask;}

		private:
			uint8_t m_ch;
			uint8_t m_mask;
		};
	}

#endif
