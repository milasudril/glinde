//@	{
//@	 "targets":[{"name":"surface.o","type":"object","pkgconfig_libs":["cairo"]}]
//@	}

#include "surface.hpp"
#include "cairohandles.hpp"

using namespace PageComposer;

Surface::Surface(int width,int height):
m_handle( cairo_image_surface_create(CAIRO_FORMAT_ARGB32,width,height) )
	{renderRegionClear();}

Surface::~Surface()
	{cairo_surface_destroy(cairosurface(m_handle));}

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

const uint8_t* Surface::data() const noexcept
	{
	auto handle=const_cast<cairo_surface_t*>(cairosurface(m_handle));
	cairo_surface_flush(handle);
	return cairo_image_surface_get_data(handle);
	}

uint8_t* Surface::data() noexcept
	{
	cairo_surface_flush(cairosurface(m_handle));
	return cairo_image_surface_get_data(cairosurface(m_handle));
	}

void Surface::dirtySet() noexcept
	{cairo_surface_mark_dirty(cairosurface(m_handle));}

void Surface::save(const char* filename) const
	{
	auto handle=const_cast<cairo_surface_t*>(cairosurface(m_handle));
	cairo_surface_write_to_png(handle,filename);
	}
