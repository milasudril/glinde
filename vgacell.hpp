/*
Glinde
Copyright (C) 2017  Torbjörn Rathsman

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/
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
