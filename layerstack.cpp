//@	{"targets":[{"name":"layerstack.o","type":"object","pkgconfig_libs":["cairo"]}]}

#include "layerstack.hpp"
#include "rendercontext.hpp"
#include "surface.hpp"
#include "cairohandles.hpp"

using namespace PageComposer;

void LayerStack::render() const noexcept
	{
	auto w=r_rc.surface().width();
	auto h=r_rc.surface().height();
	r_rc.surface().dirtyRectangleClear();

	Rectangle dirty_rect(Vec2(w,h),Vec2(0,0));
	std::for_each(m_layers.begin(),m_layers.end()
		,[&dirty_rect](const Layer& l)
			{
			if(l.dirty())
				{
				dirty_rect=boundingRectangle(dirty_rect,l.object().boundingRectOld());
				dirty_rect=boundingRectangle(dirty_rect,l.object().boundingRectangle());
				}
			}
		);
	r_rc.surface().dirtyRectangleAdd(dirty_rect);

	auto rc=cairocontext(r_rc.handle());
	cairo_set_source_rgba(rc,0,0,0,0);
	cairo_rectangle(rc,0,0,w,h);
	cairo_fill(rc);

	std::for_each(m_layers.begin(),m_layers.end()
		,[&dirty_rect](const Layer& l)
			{
			if(l.dirty() || overlap(dirty_rect,l.object().boundingRectangle()))
				{l.object().render();}
			}
		);


	}
