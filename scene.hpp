//@	{
//@	"targets":[{"name":"scene.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"scene.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_SCENE_HPP
#define GLINDE_SCENE_HPP

#include "angle/texture2d.hpp"
#include "glfwmm/size.hpp"
#include "pagecomposer/textstyle.hpp"
#include "pagecomposer/parastyle.hpp"
#include "pagecomposer/paragraph.hpp"
#include "pagecomposer/textrenderer.hpp"
#include "pagecomposer/rendercontext.hpp"
#include "pagecomposer/surface.hpp"
#include "pagecomposer/layerstack.hpp"

namespace Glinde
	{
	class Display;

	class Scene
		{
		public:
		//SRGB8_ALPHA8?
			Scene():m_hud_texture(1,Angle::TextureFormat::RGBA8,1,1)
				,m_hud_surface(1,1),m_hud_context(m_hud_surface)
				,m_hud_text(m_hud_context),m_hud_pos_display(m_hud_text)
				,m_hud_layers(m_hud_context)
				{
				m_hud_bodytext.color(PageComposer::Color(1,1,1,1));
				m_hud_pos_display.style(m_hud_bodytext);
				m_hud_para.color(PageComposer::Color(0.1,0.1,0.1,0.75));
				auto pos_style=m_hud_para;
				pos_style.alignment(PageComposer::ParaStyle::Alignment::RIGHT);
				m_hud_pos_display.style(pos_style).positionRelative(PageComposer::Vec2(1,-1))
					.anchor(PageComposer::Vec2(1,-1));
				m_hud_layers.push(PageComposer::Layer(m_hud_pos_display));
				}

			void render(Display& disp);

		private:
		//HUD stuff. Move to other class
			Angle::Texture2D m_hud_texture;
			PageComposer::Surface m_hud_surface;
			PageComposer::RenderContext m_hud_context;
			PageComposer::TextRenderer m_hud_text;
			PageComposer::ParaStyle m_hud_para;
			PageComposer::TextStyle m_hud_bodytext;
			PageComposer::Paragraph m_hud_pos_display;
			PageComposer::LayerStack m_hud_layers;

		//Keep track of old fb size
			GLFWmm::Size m_size_old;			
		};
	}

#endif
