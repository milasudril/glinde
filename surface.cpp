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
	m_dirty_data=static_cast<Pixel*>(malloc(sizeof(Pixel)*width*height));
	memset(m_dirty_data,0,sizeof(Pixel)*width*height);
	renderRegionClear();
	}

Surface::~Surface()
	{
	free(m_dirty_data);
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

Surface::Pixel* Surface::data() noexcept
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

void Surface::region_clamp() noexcept
	{
	m_dirty_region.min()=emax(Vec2{0,0},m_dirty_region.min());
	m_dirty_region.max()=emin(Vec2{double(width()),double(height())},m_dirty_region.max() );
	}

void Surface::dirty_fetch() const noexcept
	{
/*	auto data_new=data();
	auto l_0=static_cast<int>(m_dirty_region.min().x());
	auto k_0=static_cast<int>(m_dirty_region.min().y());
	auto L=m_dirty_region.widthInt();
	auto K=m_dirty_region.heightInt();
	auto w=width();
	assert(stride()==int(sizeof(Pixel)*w));
	for(int k=0;k<K;++k)
		{memcpy(m_dirty_data + k*L,data_new + (k_0 + k)*w + l_0,L*sizeof(Pixel));}*/
	m_dirty_fetched=1;
	}
