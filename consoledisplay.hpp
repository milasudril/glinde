//@	{"targets":[{"name":"consoledisplay.hpp","type":"include"}]}

#ifndef GLINDE_CONSOLEDISPLAY_HPP
#define GLINDE_CONSOLEDISPLAY_HPP

#include "renderobject.hpp"
#include "imagerenderer.hpp"
#include "consolerenderer.hpp"

namespace Glinde
	{
	class ConsoleDisplay final:public RenderObject
		{
		public:
			explicit ConsoleDisplay(const Image& charmap,Console& con):
				 m_con_renderer(charmap,con)
				,m_con_texture(10,Angle::TextureFormat::RGBA16F
					,m_con_renderer.textureWidth()
					,m_con_renderer.textureHeight())
				{
				m_con_texture.filter(Angle::MagFilter::NEAREST)
					.filter(Angle::MinFilter::NEAREST);
				}

			void framebufferResize(int width,int height){}

			void render(Display& disp) const noexcept
				{
				m_con_renderer.render(m_con_texture);
				Angle::Framebuffer::framebufferDefault()
					.bind(Angle::Framebuffer::Target::READDRAW);
				m_img_renderer.render(m_con_texture);
				}

		private:
			ConsoleRenderer m_con_renderer;
			mutable Angle::Texture2D m_con_texture;
			ImageRenderer m_img_renderer;
		};
	}

#endif
