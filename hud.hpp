//@	{
//@	"targets":[{"name":"hud.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"hud.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_HUD_HPP
#define GLINDE_HUD_HPP

#include "angle/texture2d.hpp"
#include "glfwmm/size.hpp"
#include "pagecomposer/paragraph.hpp"
#include "pagecomposer/textrenderer.hpp"
#include "pagecomposer/layerstack.hpp"

#include <memory>

namespace Glinde
	{
	class Display;

	class Hud
		{
		public:
			Hud();

			void fbResize(int width,int height);

			void render(Display& disp) const;

		private:
			mutable Angle::Texture2D m_texture;
			PageComposer::Surface m_surface;
			std::unique_ptr<PageComposer::Surface::Pixel[]> m_texbuff;
			PageComposer::RenderContext m_context;
			PageComposer::TextRenderer m_text;
			PageComposer::ParaStyle m_para;
			PageComposer::TextStyle m_bodytext;
			PageComposer::Paragraph m_pos_display;
			PageComposer::LayerStack m_layers;			
		};
	}

#endif
