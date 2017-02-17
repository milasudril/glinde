//@	{"targets":[{"name":"test","type":"application"}]}

#include "textstyle.hpp"
#include "parastyle.hpp"
#include "paragraph.hpp"
#include "textrenderer.hpp"
#include "rendercontext.hpp"
#include "surface.hpp"
#include "layerstack.hpp"


int main()
	{
	PageComposer::Surface surf(960,600);
	PageComposer::RenderContext rc(surf);
	PageComposer::TextRenderer tr(rc);

	PageComposer::ParaStyle style_para;
	style_para.color(PageComposer::Color{1,0.75,0.85,1})
		.alignment(PageComposer::ParaStyle::Alignment::CENTER);

	PageComposer::TextStyle style_text;
	style_text.size(16).family("Latin Modern Roman")
		.color(PageComposer::Color{0,0.5,0.2,1});

	auto style_shadow=style_text;
	style_shadow.color(PageComposer::Color{0.25,0.25,0.25,0.75});
	
	PageComposer::Paragraph foo(tr);
	foo.style(style_para)
		.style(style_shadow)
		.text("Flygande bäckasiner söka hwila på mjuka tufvor")
		.positionAbsolute(PageComposer::Vec2{960/2+2,2})
		.anchor(PageComposer::Vec2{0,-1});

	auto next=foo.boundingRectangle();
	
	PageComposer::Paragraph bar(tr);
	bar.style(style_para)
		.style(style_text)
		.text("0123456789")
		.positionAbsolute(next.min() + next.size())
		.anchor(PageComposer::Vec2{-1,-1});

	PageComposer::LayerStack layers;
	layers.push(PageComposer::Layer(foo))
		.push(PageComposer::Layer(bar))
		.render();


	surf.save("test.png");
	}
