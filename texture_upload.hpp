//@	{"targets":[{"name":"texture_upload.hpp","type":"include"}]}

#ifndef GLINDE_TEXTUREUPLOAD_HPP
#define GLINDE_TEXTUREUPLOAD_HPP

#include "image.hpp"
#include "errormessage.hpp"
#include "variant.hpp"
#include "angle/texture2d.hpp"

inline auto gl_type(Glinde::Image::SampleType x)
	{return GL_HALF_FLOAT;}

inline auto gl_format(Glinde::Image::SampleType x)
	{return GL_RED;}

namespace Glinde
	{
	static constexpr Angle::TextureFormat s_texture_formats[]=
		{
		 Angle::TextureFormat::R16F
		,Angle::TextureFormat::RG16F
		,Angle::TextureFormat::RGB16F
		,Angle::TextureFormat::RGBA16F
		};

	inline GLenum gl_type(ArrayFixed<Image::SampleType,2> x)
		{return gl_type(half{});}

	inline GLenum gl_type(ArrayFixed<Image::SampleType,3> x)
		{return gl_type(half{});}

	inline GLenum gl_type(ArrayFixed<Image::SampleType,4> x)
		{return gl_type(half{});}


	inline auto gl_format(ArrayFixed<Image::SampleType,2> x)
		{return GL_RG;}

	inline auto gl_format(ArrayFixed<Image::SampleType,3> x)
		{return GL_RGB;}

	inline auto gl_format(ArrayFixed<Image::SampleType,4> x)
		{return GL_RGBA;}


	inline Angle::Texture2D texture2d(const Image& img,size_t mipmaps=9)
		{
		if(img.channelCount()<1 || img.channelCount()>4)
			{throw ErrorMessage("The image has an invalid channel count.",{});}

		Angle::Texture2D ret(s_texture_formats[img.channelCount()-1]
			,img.width(),img.height());

		switch(img.channelCount())
			{
			case 1:
				ret.dataSet(img.pixels(),img.width(),img.height());
				return std::move(ret);
			case 2:
				ret.dataSet(reinterpret_cast<const ArrayFixed<half,2>*>(img.pixels())
					,img.width(),img.height());
				return std::move(ret);
			case 3:
				ret.dataSet(reinterpret_cast<const ArrayFixed<half,3>*>(img.pixels())
					,img.width(),img.height());
				return std::move(ret);
			case 4:
				ret.dataSet(reinterpret_cast<const ArrayFixed<half,4>*>(img.pixels())
					,img.width(),img.height());
				return std::move(ret);
			}
		return std::move(ret);
		}
	}

#endif