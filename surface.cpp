//@	{
//@	 "targets":[{"name":"surface.o","type":"object","pkgconfig_libs":["cairo"]}]
//@	}

#include "surface.hpp"
#include "cairohandles.hpp"
#include <cstring>
#include <cassert>

using namespace PageComposer;

Surface::Surface(int width,int height):
m_handle( cairo_image_surface_create(CAIRO_FORMAT_ARGB32,width,height) )
	{
	dirtyRectangleClear();
	}

Surface::~Surface()
	{
	cairo_surface_destroy(cairosurface(m_handle));
	}

int Surface::width() const noexcept
	{
	auto handle=const_cast<cairo_surface_t*>(cairosurface(m_handle));
	return cairo_image_surface_get_width(handle);
	}

int Surface::height() const noexcept
	{
	auto handle=const_cast<cairo_surface_t*>(cairosurface(m_handle));
	return cairo_image_surface_get_height(handle);
	}

int Surface::stride() const noexcept
	{
	auto handle=const_cast<cairo_surface_t*>(cairosurface(m_handle));
	return cairo_image_surface_get_stride(handle);
	}

const Surface::Pixel* Surface::data() const noexcept
	{
	auto handle=const_cast<cairo_surface_t*>(cairosurface(m_handle));
	cairo_surface_flush(handle);
	return reinterpret_cast<const Pixel*>(cairo_image_surface_get_data(handle));
	}

Surface::Pixel* Surface::data_nc() noexcept
	{
	cairo_surface_flush(cairosurface(m_handle));
	return reinterpret_cast<Pixel*>(cairo_image_surface_get_data(cairosurface(m_handle)));
	}

Surface& Surface::dirtySet() noexcept
	{
	cairo_surface_mark_dirty(cairosurface(m_handle));
	return *this;
	}

void Surface::save(const char* filename) const
	{
	auto handle=const_cast<cairo_surface_t*>(cairosurface(m_handle));
	cairo_surface_write_to_png(handle,filename);
	}

void Surface::rect_clamp() noexcept
	{
	m_dirty_rect.min()=emax(Vec2{0,0},m_dirty_rect.min());
	m_dirty_rect.max()=emin(Vec2(width(),height()),m_dirty_rect.max() );
	}

void Surface::rectangleGet(const Rectangle& rect,Pixel* dest) const noexcept
	{
	auto data_new=data();
	auto l_0=static_cast<int>(rect.min().x());
	auto k_0=static_cast<int>(rect.min().y());
	auto size=rect.size() + Vec2{0.5,0.5};
	auto L=static_cast<int>(size.x());
	auto K=static_cast<int>(size.y());
	auto w=width();
	assert(stride()==int(sizeof(Pixel)*w));
	for(int k=0;k<K;++k)
		{memcpy(dest + k*L,data_new + (k_0 + k)*w + l_0,L*sizeof(Pixel));}
	}
