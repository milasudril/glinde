//@	{"targets":[{"name":"test","type":"application"}]}

#include "textstyle.hpp"
#include "parastyle.hpp"
#include "paragraph.hpp"
#include "textrenderer.hpp"
#include "rendercontext.hpp"
#include "surface.hpp"
#include "layerstack.hpp"
#include <cstring>


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
		.positionRelative(PageComposer::Vec2{0,0})
		.anchor(PageComposer::Vec2{0,0})
		.render();

	auto rect=foo.boundingRectangle();

	foo.positionAbsolute(rect.min())
		.anchor(PageComposer::Vec2{1,1})
		.render();

	surf.save("test.png");

/*	auto rendered_data=surf.dataDirty();
	auto region_dirty=surf.regionDirty();
	PageComposer::Surface surf2(region_dirty.widthInt(),region_dirty.heightInt());
	memcpy(surf2.data(),rendered_data,sizeof(PageComposer::Surface::Pixel)*region_dirty.areaInt());
	surf2.dirtySet().save("test2.png");*/

	}
