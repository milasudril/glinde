//@	{
//@	 "targets":[{"name":"vgacell.o","type":"object"}]
//@	}

#include "vgacell.hpp"

using namespace Glinde;

static const char32_t* codepoints=
	U"\0☺☻♥♦♣♠•◘○◙♂♀\x1d161♬☼"
	 U"►◄↕‼¶§▬↨↑↓→←∟↔▲▼"
	U" !\"#$%&'()*+,-./"
	 U"0123456789:;<=>?"
	 U"@ABCDEFGHIJKLMNO"
	 U"PQRSTUVWXYZ[\\]^_"
	 U"`abcdefghijklmno"
	 U"pqrstuvwxyz{|}~⌂"
	 U"ÇüéâäàåçêëèïîìÄÅ"
	 U"ÉæÆôöòûùÿÖÜ¢£¥₧ƒ"
	 U"áíóúñÑªº¿⌐¬½¼¡«»"
	 U"░▒▓│┤╡╢╖╕╣║╗╝╜╛┐"
	 U"└┴┬├─┼╞╟╚╔╩╦╠═╬╧"
	 U"╨╤╥╙╘╒╓╫╪┘┌█▄▌▐▀"
	 U"αßΓπΣσµτΦΘΩδ∞φε∩"
	 U"≡±≥≤⌠⌡÷≈°∙·√ⁿ²■\0xa0";

uint32_t VGACell::codepoint() const noexcept
	{
	return codepoints[m_ch];
	}
