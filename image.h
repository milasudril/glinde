#ifdef __WAND__
target[name[image.h] type[include]]
dependency[image.o]
#endif

#ifndef GLINDA_IMAGE_H
#define GLINDA_IMAGE_H

#include <cstdint>
#include <OpenEXR/half.h>

namespace Glinda
	{
	class DataSource;

	class Image
		{
		public:
			typedef half SampleType;

			Image(DataSource&& source);
			Image(uint32_t width,uint32_t height,uint32_t n_channels);
			~Image();

			SampleType* pixelsGet() noexcept
				{return m_pixels;}

			const SampleType* pixelsGet() const noexcept
				{return m_pixels;}

			uint32_t widthGet() const noexcept
				{return m_width;}

			uint32_t heightGet() const noexcept
				{return m_width;}

		private:
			void pixelsAlloc(uint32_t width,uint32_t height,uint32_t n_channels);
			void pixelsFree();
			uint32_t m_n_channels;
			uint32_t m_width;
			uint32_t m_height;
			half* m_pixels;
		};
	}

#endif
