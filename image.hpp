/*
Glinde
Copyright (C) 2017  Torbjörn Rathsman

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/
//@	{
//@	 "dependencies_extra":[{"ref":"image.o","rel":"implementation"}]
//@	,"targets":[{"name":"image.hpp","type":"include"}]
//@	}

#ifndef GLINDE_IMAGE_HPP
#define GLINDE_IMAGE_HPP

#include "storage/arraysimple.hpp"
#include <cstdint>
#include <OpenEXR/half.h>

namespace Glinde
	{
	class DataSource;

	class Image
		{
		public:
			typedef half SampleType;

			explicit Image(DataSource&& source,uint32_t id):Image(source,id){}
			explicit Image(DataSource& source,uint32_t id);
			explicit Image(uint32_t width,uint32_t height,uint32_t n_channels
				,uint32_t id);

			SampleType* pixels() noexcept
				{return m_pixels.begin();}

			const SampleType* pixels() const noexcept
				{return m_pixels.begin();}

			uint32_t width() const noexcept
				{return m_properties.data.width;}

			uint32_t height() const noexcept
				{return m_properties.data.height;}

			uint32_t channelCount() const noexcept
				{return m_properties.data.n_channels;}

			uint32_t id() const noexcept
				{return m_properties.data.id;}

		private:
			ArraySimple<SampleType> m_pixels;
			union //Force vectorization when possible
				{
				vec4_t<uint32_t> x;
				struct
					{
					uint32_t width;
					uint32_t height;
					uint32_t n_channels;
					uint32_t id;
					} data;
				} m_properties;
		};
	}

#endif
