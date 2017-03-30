//@	{
//@	 "targets":[{"name":"consoledisplay.o","type":"object"}]
//@	}

#include "consoledisplay.hpp"
#include "../texture_upload.hpp"

using namespace Glinde;

ConsoleDisplay::ConsoleDisplay(const Console& con):
	 m_con_renderer(con)
	,m_con_texture(Angle::TextureFormat::RGBA16F
		,m_con_renderer.textureWidth()
		,m_con_renderer.textureHeight())
	{
	}
