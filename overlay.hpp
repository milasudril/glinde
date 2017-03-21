//@	{
//@	"targets":[{"name":"overlay.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"overlay.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_OVERLAY_HPP
#define GLINDE_OVERLAY_HPP

#include "stringkey.hpp"
#include "renderobject.hpp"
#include "angle/texture2d.hpp"
#include "glfwmm/size.hpp"
#include "pagecomposer/paragraph.hpp"
#include "pagecomposer/textrenderer.hpp"
#include "pagecomposer/layerstack.hpp"

#include <memory>
#include <map>

namespace Glinde
	{
	class Overlay:public RenderObject
		{
		public:
			Overlay();

			void framebufferResize(int width,int height);

			void render(Display& disp,const Timeinfo& ti) const noexcept;

			PageComposer::Paragraph& paragraph(Stringkey key) noexcept
				{
				auto i=m_paragraphs.find(key);
				if(i!=m_paragraphs.end())
					{return i->second;}
				auto ip=m_paragraphs.insert(std::pair<Stringkey,PageComposer::Paragraph>{key,PageComposer::Paragraph(m_text)});
				m_layers.push(PageComposer::Layer(ip.first->second));
				return ip.first->second;
				}

		private:
			mutable Angle::Texture2D m_texture;
			PageComposer::Surface m_surface;
			std::unique_ptr<PageComposer::Surface::Pixel[]> m_texbuff;
			PageComposer::RenderContext m_context;
			PageComposer::TextRenderer m_text;

			std::map<Stringkey,PageComposer::Paragraph> m_paragraphs;
			PageComposer::LayerStack m_layers;			
		};
	}

#endif
