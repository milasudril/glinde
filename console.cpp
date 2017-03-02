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
#include <cstdint>
#include <xmmintrin.h>

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
	getCoords(uint32_t l,uint32_t n_rows,uint32_t n_cols)
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

	inline floatpack uvcoordsGet(uint16_t ch)
		{
		auto n_cols=28;  //Character textuture must have 28 columns
		auto k=ch/n_cols;
		auto l=ch%n_cols;
		auto kf=static_cast<float>(k);
		auto lf=static_cast<float>(l);

		auto w=static_cast<float>(Console::CHARCELL_WIDTH);
		auto h=static_cast<float>(Console::CHARCELL_HEIGHT);
		floatpack size_chars {0,0,w,0,w,h,0,h};
		auto dxdy=size_chars; //Postpone normalization to shader program

		auto ret=floatpack{lf,kf,lf,kf,lf,kf,lf,kf};
		ret+=dxdy;
		return ret;
		}


template<class T>
using vec4_t __attribute__ ((vector_size(4*sizeof(T))))=vector_type(T);

	#ifdef __AVX2__
	vec4_t<float> colorGet(uint8_t color_mask)
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
		return color;
		}
	#else
	vec4_t<float> colorGet(uint8_t color_mask)
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
		return color;
		}
	#endif
	}

Console::Console(uint32_t n_rows,uint32_t n_cols):
	 m_vertices(new GeoSIMD::Point<float>[n_rows*n_cols*4])
	,m_colors_fg(new vec4_t<float>[n_rows*n_cols*4])
	,m_colors_bg(new vec4_t<float>[n_rows*n_cols*4])
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
			auto verts=getCoords(l,n_rows,n_cols);
			auto cell=l + n_cols*k;
			m_vertices[4*cell + 0]=O + verts[0];
			m_vertices[4*cell + 1]=O + verts[1];
			m_vertices[4*cell + 2]=O + verts[2];
			m_vertices[4*cell + 3]=O + verts[3];

			m_faces[2*cell + 0][0]=4*cell + 0;
			m_faces[2*cell + 0][1]=4*cell + 1;
			m_faces[2*cell + 0][2]=4*cell + 2;
			m_faces[2*cell + 1][0]=4*cell + 2;
			m_faces[2*cell + 1][1]=4*cell + 3;
			m_faces[2*cell + 1][2]=4*cell + 0;
			}
		}
	colorMask(0x07);
	}

Console& Console::colorMask(uint8_t color_mask)
	{
	m_color_fg=colorGet(color_mask&0x0f);
	m_color_bg=colorGet(static_cast<uint8_t>(color_mask>>4));
	return *this;
	}

Console& Console::writeRaw(const char* string)
	{
	while(*string!='\0')
		{
		write(*string);
		++string;
		}
	return *this;
	}

Console& Console::writeUTF8(const char* string)
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

Console& Console::write(uint32_t codepoint)
	{
	uint16_t ch=447;
	if(codepoint==10)
		{ch=65535;}
	else
	if(codepoint>=32 && codepoint<127) // Basic ASCII characters
		{ch=static_cast<uint16_t>(codepoint-32);}
	else
	if(codepoint>=160 && codepoint<256) // Latin-1 continuation
		{ch=static_cast<uint16_t>(codepoint-64);}
	else
	if(codepoint>=0x2018 && codepoint<0x2020) // Quotation marks
		{ch=static_cast<uint16_t>(codepoint-0x2018 + 192);}
	else
	if(codepoint==0x2022) //Bullet
		{ch=static_cast<uint16_t>(200);}
	else
	if(codepoint==0x203c) //Double exclamation mark
		{ch=static_cast<uint16_t>(201);}
	else
		{fprintf(stderr,"Codepoint %x missing\n",codepoint);}

	auto N=m_n_cols*m_n_rows;
	auto position=m_position;

	auto uvs=uvcoordsGet(ch);
	auto color_fg=m_color_fg;
	auto color_bg=m_color_bg;

	switch(ch)
		{
		case 65535:
			m_position=std::max(m_position
				,m_n_cols*(m_position/m_n_cols) + static_cast<size_t>(1));
			while((m_position%m_n_cols)!=0)
				{write(' ');}
			break;

		default:
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
				{++m_line_current;}

			m_position=position;
			break;
		}
	return *this;
	}

Console& Console::write(char ch)
	{
	return write(static_cast<uint32_t>( static_cast<uint8_t>(ch) ));
	}
