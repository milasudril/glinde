//@	{
//@	"targets":
//@		[{
//@		 "dependencies":[{"ref":"Half","rel":"external"}]
//@		,"name":"image.o","type":"object"
//@		,"pkgconfig_libs":["libpng"]		
//@		}]
//@	}

#include "image.hpp"
#include "datasource.hpp"
#include "errormessage.hpp"
#include "logwriter.hpp"
#include "cpuinfo.hpp"
#include "debug.hpp"
#include "narrow_cast.hpp"
#include "variant.hpp"

#include <png.h>
#include <cmath>
#include <memory>

using namespace Glinde;

namespace
	{
	class PNGReader
		{
		public:
			inline PNGReader(DataSource& source);
			inline ~PNGReader();

			uint32_t width() const noexcept
				{return m_width;}

			uint32_t height() const noexcept
				{return m_height;}

			uint32_t channelCount() const noexcept
				{return m_n_channels;}

			enum class ColorType:unsigned int
				{
				 UNKNOWN
				,INFORMATION_MISSING
				,SRGB
				,GAMMACORRECTED
				};

			inline ColorType colorType() const noexcept
				{return m_color_type;}

			inline float gamma() const noexcept
				{return static_cast<float>( m_gamma );}

			uint32_t sampleSize() const noexcept
				{return m_sample_size;}

			inline void headerRead();

			inline void pixelsRead(half* pixels_out);


		private:
			[[noreturn]] static void on_error(png_struct* pngptr,const char* message);

			static void on_warning(png_struct* pngptr,const char* message);

			static void on_read(png_struct* pngptr,uint8_t* data,png_size_t N);

			void channelBitsConversionSetup();

			png_struct* m_handle;
			png_info* m_info;
			png_info* m_info_end;

			uint32_t m_width;
			uint32_t m_height;
			uint32_t m_n_channels;
			uint32_t m_sample_size;
			double m_gamma;

			ColorType m_color_type;
			};
	};

void PNGReader::on_warning(png_struct* pngptr,const char* message)
	{
	logWrite(Log::MessageType::WARNING,"libpng: #0;",{message});
	}

void PNGReader::on_error(png_struct* pngptr,const char* message)
	{
	auto source=reinterpret_cast<DataSource*>(png_get_io_ptr(pngptr));
	throw ErrorMessage("#0;: An error occured while decoding the image. #1;"
		,{source->nameGet(),message});
	}

void PNGReader::on_read(png_struct* pngptr,uint8_t* data,png_size_t N)
	{
	auto source=reinterpret_cast<DataSource*>(png_get_io_ptr(pngptr));
	if(source->read(data,N)!=N)
		{
		throw ErrorMessage("#0;: Possible end of file while decoding the image."
			,{source->nameGet()});
		}
	}

void PNGReader::channelBitsConversionSetup()
	{
	auto n_bits=png_get_bit_depth(m_handle,m_info);

	if(n_bits < 8)
		{
		m_sample_size=1;
		png_set_packing(m_handle);
		}

	if(n_bits>=8)
		{m_sample_size=n_bits/8;}

	if(n_bits > 8)
		{
		if(!CPUInfo::bigEndianIs())
			{png_set_swap(m_handle);}
		}

	switch(png_get_color_type(m_handle,m_info))
		{
		case PNG_COLOR_TYPE_GRAY:
			m_n_channels=1;
			if(n_bits<8)
				{png_set_expand_gray_1_2_4_to_8(m_handle);}
			break;

		case PNG_COLOR_TYPE_GRAY_ALPHA:
			m_n_channels=2;
			break;

		case PNG_COLOR_TYPE_RGB:
			m_n_channels=3;
			break;

		case PNG_COLOR_TYPE_RGB_ALPHA:
			m_n_channels=4;
			break;

		case PNG_COLOR_TYPE_PALETTE:
			if(png_get_valid(m_handle,m_info,PNG_INFO_tRNS))
				{
				png_set_tRNS_to_alpha(m_handle);
				m_n_channels=4;
				}
			else
				{m_n_channels=3;}
			png_set_palette_to_rgb(m_handle);
			break;

		default:
			{
			auto source=reinterpret_cast<DataSource*>(png_get_io_ptr(m_handle));
			throw ErrorMessage("#0;: Unknown number of channels in image.",{source->nameGet()});
			}
		}
	}


PNGReader::PNGReader(DataSource& source)
	{
	m_info=nullptr;
	m_info_end=nullptr;
	m_handle=png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,on_error,on_warning);
	png_set_read_fn(m_handle,&source,on_read);
	m_info=png_create_info_struct(m_handle);
	m_color_type=ColorType::INFORMATION_MISSING;
	m_gamma=1.0;
	}

PNGReader::~PNGReader()
	{
	png_destroy_read_struct(&m_handle,&m_info,&m_info_end);
	}

void PNGReader::headerRead()
	{
	png_set_sig_bytes(m_handle,8);
	png_read_info(m_handle,m_info);
	m_width=narrow_cast<uint32_t>( png_get_image_width(m_handle,m_info) );
	m_height=narrow_cast<uint32_t>( png_get_image_height(m_handle,m_info) );
	channelBitsConversionSetup();

	if(png_get_valid(m_handle,m_info,PNG_INFO_sRGB))
		{m_color_type=ColorType::SRGB;}
	else
	if(png_get_valid(m_handle,m_info,PNG_INFO_gAMA))
		{
		png_get_gAMA(m_handle,m_info,&m_gamma);
		m_color_type=ColorType::GAMMACORRECTED;
		}
	}

template<class T>
static void pixelsScale(const T* pixels_in,Image::SampleType* pixels_out,uint32_t N)
	{
	auto factor=static_cast<float>( ( 1L<<(8L*sizeof(T)) ) - 1 );

	while(N)
		{
		*pixels_out=static_cast<float>(*pixels_in)/factor;
		--N;
		++pixels_in;
		++pixels_out;
		}
	}

void PNGReader::pixelsRead(Image::SampleType* pixels_out)
	{
	auto width=m_width;
	auto height=m_height;
	auto sample_size=m_sample_size;
	auto n_channels=m_n_channels;
	ArraySimple<uint8_t> buffer_temp(width*height*sample_size*n_channels);

		{
		auto row=buffer_temp.begin();
		ArraySimple<uint8_t*> rows
			{
			height,[row,width,sample_size,n_channels](size_t n)
				{return row + n*width*sample_size*n_channels;}
			};

		png_read_image(m_handle,rows.begin());
		}

	switch(sample_size)
		{
		case 1:
			pixelsScale(buffer_temp.begin(),pixels_out,width*height*n_channels);
			break;

		case 2:
			pixelsScale(reinterpret_cast<const uint16_t*>(buffer_temp.begin())
				,pixels_out,width*height*n_channels);
			break;
		case 4:
			pixelsScale(reinterpret_cast<const uint32_t*>(buffer_temp.begin())
				,pixels_out,width*height*n_channels);
			break;
		default:
			{
			auto source=reinterpret_cast<DataSource*>(png_get_io_ptr(m_handle));
			throw ErrorMessage("#0;: Unsupported sample size.",{source->nameGet()});
			}
		}

	}



namespace
	{
	typedef void (*ColorConverter)(Image& image);
	}

static void fromGamma(Image& image)
	{}

static float fromSRGB(float x)
	{
	return x<=0.04045f? x/12.92f : std::pow( (x + 0.055f)/(1.0f + 0.055f),2.4f);
	}

static void fromSRGB(Image& image)
	{
	auto ptr=image.pixels();
	auto n_ch=image.channelCount();
	auto N=image.width() * image.height() * n_ch;

	while(N!=0)
		{
		*ptr=fromSRGB(*ptr);
		++ptr;
		--N;
		}
	}

static ColorConverter converterGet(PNGReader::ColorType color_type)
	{
	switch(color_type)
		{
		case PNGReader::ColorType::UNKNOWN:
			logWrite(Log::MessageType::WARNING
				,"Color type for loaded image is unknown. Assuming LINEAR color values.",{});
			return nullptr;

		case PNGReader::ColorType::INFORMATION_MISSING:
			logWrite(Log::MessageType::WARNING
				,"Color type information for loaded image is missing. Assuming sRGB.",{});
			return fromSRGB;

		case PNGReader::ColorType::GAMMACORRECTED:
			return fromGamma;

		case PNGReader::ColorType::SRGB:
			return fromSRGB;

		default:
			logWrite(Log::MessageType::WARNING
				,"Color type for loaded image is unknown. Assuming LINEAR color values.",{});
			return nullptr;
		}
	}



Image::Image(DataSource& source,uint32_t id)
	{
	logWrite(Log::MessageType::INFORMATION,"Loading image from #0;"
		,{source.nameGet()});
		{
		uint8_t magic[8]="xxxxxxx";
		if(source.read(magic,8)!=8)
			{
			GLINDE_DEBUG_PRINT("magic: #0;",magic);
			throw ErrorMessage("An I/O error occured while reading the image "
				"magic number.",{});
			}

		if(png_sig_cmp(magic,0,8))
			{
			throw ErrorMessage("The image file has an unknown encoding.",{});
			}
		}

	PNGReader reader(source);
	reader.headerRead();

	m_properties.x=vec4_t<uint32_t>
		{reader.width(),reader.height(),reader.channelCount(),id};

	m_pixels=ArraySimple<SampleType>(reader.width()*reader.height()
		*reader.channelCount());

	reader.pixelsRead(m_pixels.begin());



	auto converter=converterGet(reader.colorType());
	if(converter!=nullptr)
		{
		converter(*this);
		}

	GLINDE_DEBUG_PRINT("Loaded an image of size #0; x #1;",width(),height());
	}

Image::Image(uint32_t width,uint32_t height,uint32_t n_channels,uint32_t id):
	m_pixels(width*height*n_channels)
	,m_properties{width,height,n_channels,id}
	{
	}
