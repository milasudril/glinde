//@	{"targets":[{"name":"test","type":"application"}]}

#include "textstyle.hpp"
#include "parastyle.hpp"
#include "paragraph.hpp"
#include "textrenderer.hpp"
#include "rendercontext.hpp"
#include "surface.hpp"


int main()
	{
	PageComposer::Surface surf(960,600);
	PageComposer::RenderContext rc(surf);
	PageComposer::TextRenderer tr(rc);

	PageComposer::ParaStyle style_new;
	style_new.color(PageComposer::Color{1,0.5,0.5,1})
		.alignment(PageComposer::ParaStyle::Alignment::CENTER);
	
	PageComposer::Paragraph foo(tr);
	foo.style(style_new).text("Flygande bäckasiner söka hwila på mjuka tufvor")
		.positionAbsolute(PageComposer::Vec2{960/2,0})
		.anchor(PageComposer::Vec2{0,-1})
		.render();

	foo.positionAbsolute(PageComposer::Vec2{0,0})
		.anchor(PageComposer::Vec2{-1,-1})
		.render();

	foo.positionRelative(PageComposer::Vec2{0,0})
		.anchor(PageComposer::Vec2{0,0})
		.render();

	foo.positionRelative(PageComposer::Vec2{0,1})
		.anchor(PageComposer::Vec2{0,1})
		.render();

	surf.save("test.png");
	}
