//@	{
//@	 "targets":[{"name":"surface.o","type":"object","pkgconfig_libs":["cairo"]}]
//@	}

#include "surface.hpp"
#include "cairohandles.hpp"

using namespace PageComposer;


Surface::Surface(int width,int height){}

Surface::~Surface(){}

int Surface::width() const noexcept
	{return 0;}

int Surface::height() const noexcept
	{return 0;}

int Surface::stride() const noexcept
	{return 0;}

const uint8_t* Surface::data() const noexcept
	{return nullptr;}

uint8_t* Surface::data() noexcept
	{return nullptr;}

