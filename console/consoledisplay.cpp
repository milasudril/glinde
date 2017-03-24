//@	{
//@	 "targets":[{"name":"consoledisplay.o","type":"object"}]
//@	}

#include "consoledisplay.hpp"
#include "../texture_upload.hpp"

using namespace Glinde;

ConsoleDisplay::ConsoleDisplay(const Image& charmap,Console& con):
	 m_charmap(texture2d(charmap,1))
	,m_con_renderer(m_charmap,con)
	,m_con_texture(10,Angle::TextureFormat::RGBA16F
		,m_con_renderer.textureWidth()
		,m_con_renderer.textureHeight())
	{
	m_charmap.filter(Angle::MagFilter::NEAREST)
		.filter(Angle::MinFilter::NEAREST);
	}
