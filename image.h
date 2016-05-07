#ifdef __WAND__
target[name[image.h] type[include]]
dependency[image.o]
#endif

#ifndef GLINDE_IMAGE_H
#define GLINDE_IMAGE_H

#include "arraysimple.h"
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

			SampleType* pixelsGet() noexcept
				{return m_pixels.begin();}

			const SampleType* pixelsGet() const noexcept
				{return m_pixels.begin();}

			uint32_t widthGet() const noexcept
				{return m_properties.data.width;}

			uint32_t heightGet() const noexcept
				{return m_properties.data.height;}

			uint32_t channelCountGet() const noexcept
				{return m_properties.data.n_channels;}

			uint32_t idGet() const noexcept
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
