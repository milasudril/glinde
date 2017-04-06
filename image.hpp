//@	{
//@	 "dependencies_extra":[{"ref":"image.o","rel":"implementation"}]
//@	,"targets":[{"name":"image.hpp","type":"include"}]
//@	}

#ifndef GLINDE_IMAGE_HPP
#define GLINDE_IMAGE_HPP

#include "arraysimple.hpp"
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
