//@	{"targets":[{"name":"test","type":"application"}]}

#include "textstyle.hpp"
#include "parastyle.hpp"
#include "paragraph.hpp"
#include "textrenderer.hpp"
#include "rendercontext.hpp"
#include "surface.hpp"
#include "layerstack.hpp"
#include <cstring>
#include <cassert>

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
	
	PageComposer::Paragraph foo(tr);
	foo.style(style_para)
		.style(style_text)
		.text("Flygande bäckasiner söka hwila på mjuka tufvor")
		.positionRelative(PageComposer::Vec2{1,1})
		.anchor(PageComposer::Vec2{0,0});

	PageComposer::LayerStack layers(rc);
	layers.push(PageComposer::Layer(foo));

	for(size_t k=0;k<2;++k)
		{
		layers.render();
		surf.save("test.png");

		auto rect=surf.dirtyRectangle();
		auto size=rect.size();
		if(size.x()>0)
			{
			assert(k!=1);
			auto si=rect.sizeInt();
			PageComposer::Surface surf2(si.x(),si.y());
			surf2.contentModify([&surf,si](PageComposer::Surface::Pixel* pixels,int width,int height)
				{
				surf.rectangleGet(surf.dirtyRectangle(),pixels);
				}).save("test2.png");
			}
		}
	}
