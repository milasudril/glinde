//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"console.o",
//@	            "type":"object"
//@	        }
//@	    ]
//@	}
#include "console.hpp"
#include "arrayfixed.hpp"
#include "vgacell.hpp"
#include <cstdint>
#include <xmmintrin.h>
#include <limits>

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


	static constexpr uint32_t UTF8_ACCEPT=0;
	static constexpr uint32_t UTF8_REJECT=1;

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

	uint32_t inline
	decode(uint32_t& state, uint32_t& codep, int8_t byte)
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

namespace
	{
	using floatpack __attribute__ ((vector_size(8*sizeof(float))))=vector_type(float);

	inline ArrayFixed<GeoSIMD::Vector<float>,4>
	coords(uint32_t l,uint32_t n_rows,uint32_t n_cols) noexcept
		{
	//TODO: vectorize more if possible
		auto dx=1.0f/static_cast<float>(n_cols);
		auto dy=1.0f/static_cast<float>(n_rows);

		auto x=static_cast<float>(l)*dx;
		auto y=1.0f; //Scrolling is done in shader program;

		ArrayFixed<GeoSIMD::Vector<float>,4> coords
			(
			 2.0f*(GeoSIMD::Point<float>(x,y,0.0f)-GeoSIMD::Point<float>(0.5f,0.5f,0.0f))
			,2.0f*(GeoSIMD::Point<float>(x+dx,y,0.0f)-GeoSIMD::Point<float>(0.5f,0.5f,0.0f))
			,2.0f*(GeoSIMD::Point<float>(x+dx,y-dy,0.0f)-GeoSIMD::Point<float>(0.5f,0.5f,0.0f))
			,2.0f*(GeoSIMD::Point<float>(x,y-dy,0.0f)-GeoSIMD::Point<float>(0.5f,0.5f,0.0f))
			);
			return coords;
		}

	inline floatpack uvcoords(uint16_t ch) noexcept
		{
		auto n_cols=28;  //Character textuture must have 28 columns
		auto k=ch/n_cols;
		auto l=ch%n_cols;
		auto kf=static_cast<float>(k);
		auto lf=static_cast<float>(l);

		floatpack size_chars{0,0,1,0,1,1,0,1};
		auto dxdy=size_chars; //Postpone normalization to shader program

		auto ret=floatpack{lf,kf,lf,kf,lf,kf,lf,kf};
		ret+=dxdy;
		return ret;
		}

	#ifdef __AVX2__
	Color color(uint8_t color_mask) noexcept
		{
		vec4_t<float> color;
		auto intensity=0.3333f*static_cast<float>( (color_mask & 8)/8 );
		vec4_t<uint32_t> masks{4u,2u,1u,0};
		masks&=color_mask;
		masks>>=vec4_t<uint32_t>{2,1,0,1};
		vec4_t<float> cvec=_mm_cvtepi32_ps(reinterpret_cast<__m128i>(masks));
		color=cvec*0.6667f + vec4_t<float>{intensity,intensity,intensity,intensity};
		// Adjust the green level
		color=(color_mask==6)?color*vec4_t<float>{1.0f,0.5,1.0f,1.0f}:color;
		// make it opaque 
		color[3]=1.0f;
		return Color::fromSRGB(color);
		}
	#else
	Color color(uint8_t color_mask) noexcept
		{
		vec4_t<float> color;
		auto intensity=0.3333f*static_cast<float>( (color_mask & 8)/8 );
		vec4_t<uint32_t> masks{4u,2u,1u,0};
		masks&=color_mask;
		vec4_t<float> cvec=_mm_cvtepi32_ps(reinterpret_cast<__m128i>(masks));
		color=cvec*0.6667f*vec4_t<float>{0.25,0.5,1,1} + vec4_t<float>{intensity,intensity,intensity,intensity};
		// Adjust the green level
		color=(color_mask==6)?color*vec4_t<float>{1.0f,0.5,1.0f,1.0f}:color;
		// make it opaque 
		color[3]=1.0f;
		return Color::fromSRGB(color);
		}
	#endif
	}

Console::Console(uint32_t n_rows,uint32_t n_cols):
	 m_vertices(new GeoSIMD::Point<float>[n_rows*n_cols*4])
	,m_colors_fg(new Color[n_rows*n_cols*4])
	,m_colors_bg(new Color[n_rows*n_cols*4])
	,m_uvs(new vec2_t<float>[n_rows*n_cols*4])
	,m_faces(new FaceIndirect[2*n_rows*n_cols])
	,m_n_cols(n_cols),m_position(0),m_line_current(0)
	,m_utf8_state(0),m_codepoint(0)
	{
	auto O=GeoSIMD::origin<float>();
	for(uint32_t k=0;k<n_rows;++k)
		{
		for(uint32_t l=0;l<n_cols;++l)
			{
			auto verts=coords(l,n_rows,n_cols);
			auto cell=l + n_cols*k;
			m_vertices[4*cell + 0]=O + verts[0];
			m_vertices[4*cell + 1]=O + verts[1];
			m_vertices[4*cell + 2]=O + verts[2];
			m_vertices[4*cell + 3]=O + verts[3];

			m_faces[2*cell + 0][2]=4*cell + 0;
			m_faces[2*cell + 0][1]=4*cell + 1;
			m_faces[2*cell + 0][0]=4*cell + 2;
			m_faces[2*cell + 1][2]=4*cell + 2;
			m_faces[2*cell + 1][1]=4*cell + 3;
			m_faces[2*cell + 1][0]=4*cell + 0;
			}
		}
	m_n_rows=n_rows;
	m_n_cols=n_cols;
	colorMask(0x07);
	}

Console& Console::colorMask(uint8_t color_mask) noexcept
	{
	m_color_fg=color(color_mask&0x0f);
	m_color_bg=color(static_cast<uint8_t>(color_mask>>4));
	return *this;
	}

Console& Console::writeRaw(const char* string) noexcept
	{
	while(*string!='\0')
		{
		write(*string);
		++string;
		}
	return *this;
	}

Console& Console::writeUTF8(const char* string) noexcept
	{
	if(string==nullptr)
		{
		writeRaw("(null)");
		return *this;
		}
	auto cp=m_codepoint;
	auto state=m_utf8_state;
	while(*string!='\0')
		{
		if(decode(state,cp,*string)==0)
			{write(cp);}
		++string;
		}
	m_codepoint=cp;
	assert(state==UTF8_ACCEPT);
	m_utf8_state=state;

	return *this;
	}

static constexpr auto CONTROLCODE=std::numeric_limits<uint16_t>::max() - 511;

static constexpr uint16_t charmap_misc(uint32_t codepoint)
	{
	switch(codepoint)
		{
		case '\0': return 0;
		case u'☺': return 1;
		case u'☻': return 2;
		case u'♥': return 3;
		case u'♦': return 4;
		case u'♣': return 5;
		case u'♠': return 6;
		case u'•': return 7;
		case u'◘': return 8;
		case u'○': return 9;
		case u'◙': return 10;
		case u'♂': return 11;
		case u'♀': return 12;
		case U'𝅘𝅥𝅯': return 13;
		case u'♬': return 14;
		case u'☼': return 15;
		case u'►': return 16;
		case u'◄': return 17;
		case u'↕': return 18;
		case u'‼': return 19;
		case u'§': return u'§'; //§ exists in latin-1
		case u'¶': return u'¶'; //¶ exists in latin-1
		case u'▬': return 22;
		case u'↨': return 23;
		case u'↑': return 24;
		case u'↓': return 25;
		case u'→': return 26;
		case u'←': return 27;
		case u'∟': return 28;
		case u'↔': return 29;
		case u'▲': return 30;
		case u'▼': return 31;
		case u'⌂': return 127;
		case u'░': return 403;
		case u'▒': return 404;
		case u'▓': return 405;
		case u'│': return 406;
		case u'┤': return 407;
		case u'╡': return 408;
		case u'╢': return 409;
		case u'╖': return 410;
		case u'╕': return 411;
		case u'╣': return 412;
		case u'║': return 413;
		case u'╗': return 414;
		case u'╝': return 415;
		case u'╜': return 416;
		case u'╛': return 417;
		case u'┐': return 418;
		case u'└': return 419;
		case u'┴': return 420;
		case u'┬': return 421;
		case u'├': return 422;
		case u'─': return 423;
		case u'┼': return 424;
		case u'╞': return 425;
		case u'╟': return 426;
		case u'╚': return 427;
		case u'╔': return 428;
		case u'╩': return 429;
		case u'╦': return 430;
		case u'╠': return 431;
		case u'═': return 432;
		case u'╬': return 433;
		case u'╧': return 434;
		case u'╨': return 435;
		case u'╤': return 436;
		case u'╥': return 437;
		case u'╙': return 438;
		case u'╘': return 439;
		case u'╒': return 440;
		case u'╓': return 441;
		case u'╫': return 442;
		case u'╪': return 443;
		case u'┘': return 444;
		case u'┌': return 445;
		case u'█': return 446;
		}
	return 447;
	}

static constexpr uint16_t charmap(uint32_t codepoint) noexcept
	{
	if(codepoint==0x2264) //leq
		{return 20;}

	if(codepoint==0x2265) //geq
		{return 21;}

	if(codepoint<32) //Control codes
		{return static_cast<uint16_t>(codepoint) + CONTROLCODE;}

	if(codepoint>=32 && codepoint<128) //ASCII characters
		{return static_cast<uint16_t>(codepoint);}

	if(codepoint>=0xe000 && codepoint<0xe100) //First 256 elements of private area (used for attribute change)
		{return static_cast<uint16_t>(codepoint-0xe000) + CONTROLCODE + 256;}
	
	if(codepoint>=160 && codepoint<256) //Latin-1 characters
		{return static_cast<uint16_t>(codepoint - 32);}

	if(codepoint>=0x384 && codepoint<0x3ce) //Greek characters
		{return static_cast<uint16_t>(codepoint - 0x384) + 224;}

	if(codepoint>=0x2018 && codepoint<0x2020) // Quotation marks
		{return static_cast<uint16_t>(codepoint-0x2018) + 395;}

	//Misc dingbats, box drawing, and blocks
	return charmap_misc(codepoint); 
	}

Console& Console::write(uint32_t codepoint) noexcept
	{
	auto ch=charmap(codepoint);
	auto N=m_n_cols*m_n_rows;
	fprintf(stderr,"Write: (%zu,%zu)\n",m_line_current,m_position%m_n_cols);	if(ch>=CONTROLCODE)
		{
		if(ch>=CONTROLCODE+256)
			{colorMask(ch - (CONTROLCODE+256));}
		
		if(ch==CONTROLCODE + 10)
			{
			++m_line_current;
			m_position=(m_line_current*m_n_cols)%N;
			}
		if(ch==CONTROLCODE + 13)
			{m_position=(m_line_current*m_n_cols)%N;}
		return *this;
		}

	if(ch==447)
		{fprintf(stderr,"Codepoint %x missing\n",codepoint);}

	auto position=m_position;

	auto uvs=uvcoords(ch);
	auto color_fg=m_color_fg;
	auto color_bg=m_color_bg;

	m_uvs[4*position + 0]=vec2_t<float>{uvs[0],uvs[1]};
	m_uvs[4*position + 1]=vec2_t<float>{uvs[2],uvs[3]};
	m_uvs[4*position + 2]=vec2_t<float>{uvs[4],uvs[5]};
	m_uvs[4*position + 3]=vec2_t<float>{uvs[6],uvs[7]};

	m_colors_fg[4*position + 0]=color_fg;
	m_colors_fg[4*position + 1]=color_fg;
	m_colors_fg[4*position + 2]=color_fg;
	m_colors_fg[4*position + 3]=color_fg;

	m_colors_bg[4*position + 0]=color_bg;
	m_colors_bg[4*position + 1]=color_bg;
	m_colors_bg[4*position + 2]=color_bg;
	m_colors_bg[4*position + 3]=color_bg;

	position=(position + 1)%N;

	if(position%m_n_cols==0)
		{scroll_down();}

	m_position=position;
	return *this;
	}

Console& Console::write(char ch) noexcept
	{
	return write(static_cast<uint32_t>( static_cast<uint8_t>(ch) ));
	}

Console& Console::writeVGADump(Range<const VGACell> dump) noexcept
	{
	auto ptr=dump.begin();
	uint8_t mask_prev=0;
	while(ptr!=dump.end())
		{
		auto mask=ptr->colorMask();
		if(mask!=mask_prev)
			{
			mask_prev=mask;
			colorMask(mask);
			}
		write(ptr->codepoint());
		++ptr;
		}
	return *this;
	}

