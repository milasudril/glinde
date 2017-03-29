//@	{"targets":[{"name":"viewpoint.hpp","type":"include"}]}

#ifndef GLINDE_VIEWPOINT_HPP
#define GLINDE_VIEWPOINT_HPP

#include <geosimd/rotation.hpp>
#include <geosimd/point.hpp>

namespace Glinde
	{
	struct Viewpoint
		{
		GeoSIMD::Rotation<float> orientation;
		GeoSIMD::Point<float> location;
		GeoSIMD::Angle<float> fov_y;
		};
	}

#endif