//@	{"targets":[{"name":"consolebuffer.o","type":"object"}]}
#include "consolebuffer.hpp"
#include "../io/utf8.hpp"
#include "../storage/arrayfixed.hpp"
#include "../vgacell.hpp"
#include <cstdint>
#include <xmmintrin.h>
#include <limits>

using namespace Glinde;

namespace
	{
#ifdef __AVX__
	using floatpack __attribute__ ((vector_size(8*sizeof(float))))=vector_type(float);
#else
	struct floatpack
		{
		constexpr float operator[](int k) const noexcept
			{
			return k<4?a[0][k]:a[1][k-4];
			}

		floatpack& operator+=(const floatpack& b) noexcept
			{
			a[0]+=b.a[0];
			a[1]+=b.a[1];
			return *this;
			}
			
		vec4_t<float> a[2];
		};
#endif
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
		auto n_cols=32;  //Character textuture must have 32 columns
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
	}

ConsoleBuffer::ConsoleBuffer(uint32_t n_rows,uint32_t n_cols):
	 m_vertices(new GeoSIMD::Point<float>[(n_rows*n_cols + 1)*4])
	,m_colors(new uint32_t[(n_rows*n_cols + 1)*4])
	,m_uvs(new vec2_t<float>[(n_rows*n_cols + 1)*4])
	,m_faces(new FaceIndirect<>[2*(n_rows*n_cols + 1)])
	,m_n_cols(n_cols),m_position(0),m_line_current(0)
	,m_utf8_state(0),m_codepoint(0),m_full(0),m_scroll_pending(0)
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

			m_faces[2*cell + 0][2]=static_cast<uint16_t>( 4*cell + 0 );
			m_faces[2*cell + 0][1]=static_cast<uint16_t>( 4*cell + 1 );
			m_faces[2*cell + 0][0]=static_cast<uint16_t>( 4*cell + 2 );
			m_faces[2*cell + 1][2]=static_cast<uint16_t>( 4*cell + 2 );
			m_faces[2*cell + 1][1]=static_cast<uint16_t>( 4*cell + 3 );
			m_faces[2*cell + 1][0]=static_cast<uint16_t>( 4*cell + 0 );
			}
		}
		{
		auto verts=coords(0,n_rows,n_cols);
		auto cell=n_rows*n_cols;
		m_vertices[4*cell + 0]=O + verts[0];
		m_vertices[4*cell + 1]=O + verts[1];
		m_vertices[4*cell + 2]=O + verts[2];
		m_vertices[4*cell + 3]=O + verts[3];
		m_faces[2*cell + 0][2]=static_cast<uint16_t>( 4*cell + 0 );
		m_faces[2*cell + 0][1]=static_cast<uint16_t>( 4*cell + 1 );
		m_faces[2*cell + 0][0]=static_cast<uint16_t>( 4*cell + 2 );
		m_faces[2*cell + 1][2]=static_cast<uint16_t>( 4*cell + 2 );
		m_faces[2*cell + 1][1]=static_cast<uint16_t>( 4*cell + 3 );
		m_faces[2*cell + 1][0]=static_cast<uint16_t>( 4*cell + 0 );
		}

	m_n_rows=n_rows;
	m_n_cols=n_cols;
	colorMask(0xc);
	}

ConsoleBuffer& ConsoleBuffer::writeRaw(const char* string) noexcept
	{
	while(*string!='\0')
		{
		write(*string);
		++string;
		}
	return *this;
	}

ConsoleBuffer& ConsoleBuffer::writeUTF8(const char* string) noexcept
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
		if(utf8_decode(state,cp,*string)==0)
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
		case U'\x1d161': return 13;
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
		case u'〜': return 452;
		case u'✦': return 453;
		case u'✧': return 454;
		case u'░': return 463;
		case u'▒': return 464;
		case u'▓': return 465;
		case u'│': return 466;
		case u'┤': return 467;
		case u'╡': return 468;
		case u'╢': return 469;
		case u'╖': return 470;
		case u'╕': return 471;
		case u'╣': return 472;
		case u'║': return 473;
		case u'╗': return 474;
		case u'╝': return 475;
		case u'╜': return 476;
		case u'╛': return 477;
		case u'┐': return 478;
		case u'└': return 479;
		case u'┴': return 480;
		case u'┬': return 481;
		case u'├': return 482;
		case u'─': return 483;
		case u'┼': return 484;
		case u'╞': return 485;
		case u'╟': return 486;
		case u'╚': return 487;
		case u'╔': return 488;
		case u'╩': return 489;
		case u'╦': return 490;
		case u'╠': return 491;
		case u'═': return 492;
		case u'╬': return 493;
		case u'╧': return 494;
		case u'╨': return 495;
		case u'╤': return 496;
		case u'╥': return 497;
		case u'╙': return 498;
		case u'╘': return 499;
		case u'╒': return 500;
		case u'╓': return 501;
		case u'╫': return 502;
		case u'╪': return 503;
		case u'┘': return 504;
		case u'┌': return 505;
		case u'█': return 506;
		case u'▄': return 507;
		case u'▌': return 508;
		case u'▐': return 509;
		case u'▀': return 510;
		}
	return 511;
	}

static constexpr uint16_t charmap(uint32_t codepoint) noexcept
	{
	if(codepoint==0x2264) //leq
		{return 20;}

	if(codepoint==0x2265) //geq
		{return 21;}

	if(codepoint<32) //Control codes
		{return static_cast<uint16_t>(codepoint + CONTROLCODE);}

	if(codepoint>=32 && codepoint<128) //ASCII characters
		{return static_cast<uint16_t>(codepoint);}

	if(codepoint>=0xe000 && codepoint<0xe100) //First 256 elements of private area (used for attribute change)
		{return static_cast<uint16_t>((codepoint-0xe000) + CONTROLCODE + 256);}
	
	if(codepoint>=160 && codepoint<256) //Latin-1 characters
		{return static_cast<uint16_t>(codepoint - 32);}

	if(codepoint>=0x384 && codepoint<0x3ce) //Greek characters
		{return static_cast<uint16_t>((codepoint - 0x384) + 224);}

	if(codepoint>=0x2018 && codepoint<0x2020) // Quotation marks
		{return static_cast<uint16_t>((codepoint-0x2018) + 459);}

	//Misc dingbats, box drawing, and blocks
	return charmap_misc(codepoint); 
	}

ConsoleBuffer& ConsoleBuffer::write(uint32_t codepoint) noexcept
	{
	auto ch=charmap(codepoint);
	auto N=m_n_cols*m_n_rows;

	if(ch>=CONTROLCODE)
		{
		if(ch>=CONTROLCODE + 256)
			{colorMask(static_cast<uint8_t>(ch - (CONTROLCODE+256)));}
		
		if(ch==CONTROLCODE + 10)
			{
			if(m_scroll_pending)
				{scroll_down();}
			else
				{position_advance_newline();}
			}

		if(ch==CONTROLCODE + 13)
			{
			auto line_current=m_position/m_n_cols;
			m_position=(line_current*m_n_cols)%N;
			}
		return *this;
		}

	if(ch==511)
		{fprintf(stderr,"Codepoint %x missing\n",codepoint);}

	if(m_scroll_pending)
		{scroll_down();}

	character_render(ch,m_position);
	position_advance();
	return *this;
	}

void ConsoleBuffer::character_render(uint16_t ch,size_t position)
	{
	auto uvs=uvcoords(ch);

	m_uvs[4*position + 0]=vec2_t<float>{uvs[0],uvs[1]};
	m_uvs[4*position + 1]=vec2_t<float>{uvs[2],uvs[3]};
	m_uvs[4*position + 2]=vec2_t<float>{uvs[4],uvs[5]};
	m_uvs[4*position + 3]=vec2_t<float>{uvs[6],uvs[7]};
	m_colors[4*position + 0]=m_color;
	m_colors[4*position + 1]=m_color;
	m_colors[4*position + 2]=m_color;
	m_colors[4*position + 3]=m_color;
	}

void ConsoleBuffer::scroll_down() noexcept
	{
	auto pos=m_position;
	auto n_cols=m_n_cols;
	for(decltype(n_cols) k=0;k<n_cols;++k)
		{character_render(32,k + pos);}

	m_line_current=m_line_current<m_n_rows-1? m_line_current + 1 : 0;
	m_scroll_pending=0;
	}

ConsoleBuffer& ConsoleBuffer::write(char ch) noexcept
	{
	return write(static_cast<uint32_t>( static_cast<uint8_t>(ch) ));
	}

ConsoleBuffer& ConsoleBuffer::writeVGADump(Range<const VGACell> dump) noexcept
	{
	auto ptr=dump.begin();
	uint8_t mask_prev=0;
	size_t position=0;
	auto N=size();
	while(ptr!=dump.end() && position!=N)
		{
		auto mask=ptr->colorMask();
		if(mask!=mask_prev)
			{
			mask_prev=mask;
			colorMask(mask);
			}
		auto ch=charmap(ptr->codepoint());
		if(ch<CONTROLCODE) // Render control codes would be confusing
			{
			character_render(ch,position);
			++position;
			}
		++ptr;
		}
	if(position==N)
		{
		m_full=1;
		m_scroll_pending=1;
		position=0;
		}
	m_position=position;
	m_line_current=0;
	return *this;
	}

ConsoleBuffer& ConsoleBuffer::fill(int n,uint32_t ch) noexcept
	{
	while(n)
		{
		write(ch);
		--n;
		}
	return *this;
	}

