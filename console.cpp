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
#include "console.h"
#include "arrayfixed.h"
#include "blob.h"
#include "memoryreader.h"

using namespace Glinde;

GLINDE_BLOB(charmap,"charmap.png");

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

inline static ArrayFixed<glm::vec3,4>
getCoords(uint32_t k,uint32_t l,uint32_t n_rows,uint32_t n_cols)
	{
	ArrayFixed<glm::vec3,4> coords;
	auto dx=1.0f/static_cast<float>(n_cols);
	auto dy=1.0f/static_cast<float>(n_rows);

	auto x=static_cast<float>(l)*dx;
	auto y=1.0f;// - static_cast<float>(k)*dy; This has to be done in the shader

	coords[0]=2.0f*(glm::vec3(x,y,0.0f)-glm::vec3(0.5f,0.5f,0.0f));
	coords[1]=2.0f*(glm::vec3(x+dx,y,0.0f)-glm::vec3(0.5f,0.5f,0.0f));
	coords[2]=2.0f*(glm::vec3(x+dx,y-dy,0.0f)-glm::vec3(0.5f,0.5f,0.0f));
	coords[3]=2.0f*(glm::vec3(x,y-dy,0.0f)-glm::vec3(0.5f,0.5f,0.0f));

	return coords;
	}

Console::Console(uint32_t n_rows,uint32_t n_cols):
	 m_fontmap(MemoryReader(charmap_begin,charmap_end),0xffffffff)
	,m_vertices(n_rows*n_cols*4)
	,m_colors_fg(n_rows*n_cols*4)
	,m_colors_bg(n_rows*n_cols*4)
	,m_uvs(n_rows*n_cols*4)
	,m_faces(2*3*n_rows*n_cols),m_n_cols(n_cols),m_position(0),m_line_current(0)
	,m_utf8_state(0),m_codepoint(0)
	{
	for(uint32_t k=0;k<n_rows;++k)
		{
		for(uint32_t l=0;l<n_cols;++l)
			{
			auto verts=getCoords(k,l,n_rows,n_cols);
			auto cell=l + n_cols*k;
			m_vertices[4*cell + 0]=verts[0];
			m_vertices[4*cell + 1]=verts[1];
			m_vertices[4*cell + 2]=verts[2];
			m_vertices[4*cell + 3]=verts[3];

			m_faces[2*cell + 0][0]=4*cell + 0;
			m_faces[2*cell + 0][1]=4*cell + 1;
			m_faces[2*cell + 0][2]=4*cell + 2;
			m_faces[2*cell + 1][0]=4*cell + 2;
			m_faces[2*cell + 1][1]=4*cell + 3;
			m_faces[2*cell + 1][2]=4*cell + 0;
			}
		}
	memset(m_colors_fg.begin(),0,sizeof(glm::vec3)*n_rows*n_cols);
	memset(m_colors_bg.begin(),0,sizeof(glm::vec3)*n_rows*n_cols);
	colorMaskSet(0x07);
	}



inline static ArrayFixed<glm::vec2,4>
getCoordsUV(uint16_t ch,uint32_t width,uint32_t height)
	{
	ArrayFixed<glm::vec2,4> coords;

	auto w=static_cast<float>(width);
	auto h=static_cast<float>(height);

	auto dx=static_cast<float>(Console::CHARCELL_WIDTH)/w;
	auto dy=static_cast<float>(Console::CHARCELL_HEIGHT)/h;

	auto n_cols=28;  //Character textuture must have 28 columns
	auto k=ch/n_cols;
	auto l=ch%n_cols;

	auto x=static_cast<float>(l)*dx;
	auto y=static_cast<float>(k)*dy;

	coords[0]=glm::vec2(x,y);
	coords[1]=glm::vec2(x+dx,y);
	coords[2]=glm::vec2(x+dx,y+dy);
	coords[3]=glm::vec2(x,y+dy);

	return coords;
	}

inline glm::vec3 colorGet(uint8_t color_mask)
	{
	glm::vec3 color;
	auto intensity=0.33f*static_cast<float>( (color_mask & 8)/8 );

	color.r=0.66f*static_cast<float>( (color_mask & 4)/4 ) + intensity;
	color.g=0.66f*static_cast<float>( (color_mask & 2)/2 ) + intensity;
	color.b=0.66f*static_cast<float>( (color_mask & 1)/1 ) + intensity;

//	Adjust the green level
	color.g=(color_mask==6)?0.5f*color.g:color.g;

	return color;
	}

Console& Console::colorMaskSet(uint8_t color_mask)
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

	auto N=m_uvs.length()/4;
	auto position=m_position;
	auto w=m_fontmap.widthGet();
	auto h=m_fontmap.heightGet();

	auto uvs=getCoordsUV(ch,w,h);
	auto color_fg=m_color_fg;
	auto color_bg=m_color_bg;

	switch(ch)
		{
		case 65535:
			m_position=std::max(m_position
				,m_n_cols*(m_position/m_n_cols) + static_cast<size_t>(1));
			while((m_position%m_n_cols)!=0)
				{
				write(' ');
				}
			break;

		default:
			m_uvs[4*position + 0]=uvs[0];
			m_uvs[4*position + 1]=uvs[1];
			m_uvs[4*position + 2]=uvs[2];
			m_uvs[4*position + 3]=uvs[3];

			m_colors_fg[4*position + 0]=color_fg;
			m_colors_fg[4*position + 1]=color_fg;
			m_colors_fg[4*position + 2]=color_fg;
			m_colors_fg[4*position + 3]=color_fg;

			m_colors_bg[4*position + 0]=color_bg;
			m_colors_bg[4*position + 1]=color_bg;
			m_colors_bg[4*position + 2]=color_bg;
			m_colors_bg[4*position + 3]=color_bg;

			position=(position + 1)%N;

			if(position%colsCountGet()==0)
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
