//@	{
//@	 "targets":[{"name":"utf8.o","type":"object"}]
//@	}

#include "utf8.hpp"

using namespace Glinde;

namespace //Code from Björn Höhrmann
	{
//	Copyright (c) 2008-2009 Björn Höhrmann <bjoern@hoehrmann.de>
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to
//	deal in the Software without restriction, including without limitation the
//	rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
//	sell copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//	IN THE SOFTWARE.
//
//	See http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details.

	static constexpr uint8_t UTF8_STATES[]=
		{
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 00..1f
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 20..3f
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 40..5f
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 60..7f
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, // 80..9f
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, // a0..bf
		8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, // c0..df
		0xa,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x4,0x3,0x3, // e0..ef
		0xb,0x6,0x6,0x6,0x5,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8, // f0..ff
		0x0,0x1,0x2,0x3,0x5,0x8,0x7,0x1,0x1,0x1,0x4,0x6,0x1,0x1,0x1,0x1, // s0..s0
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1, // s1..s2
		1,2,1,1,1,1,1,2,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1, // s3..s4
		1,2,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,3,1,1,1,1,1,1, // s5..s6
		1,3,1,1,1,1,1,3,1,3,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // s7..s8
		};

	inline uint32_t decode(uint32_t& state, uint32_t& codep, int8_t byte) noexcept
		{
		auto x=static_cast<uint8_t>(byte);
		uint32_t type = UTF8_STATES[x];
		codep = (state != UTF8_ACCEPT) ?
			(x & 0x3fu) | (codep << 6) :
			(0xff >> type) & (x);
		state = UTF8_STATES[256 + state*16 + type];
		return state;
		}
	}
//	End Höhrmann's code

uint32_t Glinde::utf8_decode(uint32_t& state, uint32_t& codep, int8_t byte) noexcept
	{return decode(state,codep,byte);}

ArrayDynamic<uint32_t> Glinde::utf8_decode(const char* str)
	{
	ArrayDynamic<uint32_t> ret;
	uint32_t state=0;
	uint32_t cp=0;
	while(1)
		{
		auto ch_in=*str;
		if(ch_in=='\0')
			{return std::move(ret);}
		if(decode(state,cp,ch_in)==0)
			{ret.append(cp);}
		++ch_in;
		}
	}

String Glinde::utf8_encode(Range<const uint32_t> src)
	{
	String dest;
	auto ptr_in=src.begin();
	while(ptr_in!=src.end())
		{
		auto code_point=*ptr_in;
		if (code_point < 0x80)
			{dest.append(static_cast<char>(code_point));}
		else
		if(code_point <= 0x7FF)
			{
			dest.append(static_cast<char>((code_point >> 6) + 0xC0))
				.append(static_cast<char>((code_point & 0x3F) + 0x80));
			}
		else
		if(code_point <= 0xFFFF)
			{
			dest.append(static_cast<char>((code_point >> 12) + 0xE0))
				.append(static_cast<char>(((code_point >> 6) & 0x3F) + 0x80))
				.append(static_cast<char>((code_point & 0x3F) + 0x80));
			}
		else
		if(code_point <= 0x10FFFF)
			{
			dest.append(static_cast<char>((code_point >> 18) + 0xF0))
				.append(static_cast<char>(((code_point >> 12) & 0x3F) + 0x80))
				.append(static_cast<char>(((code_point >> 6) & 0x3F) + 0x80))
				.append(static_cast<char>((code_point & 0x3F) + 0x80));
			}
		else
			{throw ErrorMessage("Codepoint #0; is invalid",{code_point});}

		++ptr_in;
		}
	return std::move(dest);
	}
