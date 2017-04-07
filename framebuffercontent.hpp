//@	{
//@	 "targets":[{"name":"framebuffercontent.hpp","type":"include"}]
//@	}

#ifndef GLINDE_FRAMEBUFFERCONTENT_HPP
#define GLINDE_FRAMEBUFFERCONTENT_HPP

#include "storage/arrayfixed.hpp"
#include "angle/texture2d.hpp"

namespace Glinde
	{
	template<Angle::TextureFormat...formats>
	class FramebufferContent
		{
		public:
			FramebufferContent():m_textures(std::move(Angle::Texture2D(formats,1,1))...)
				{}

			~FramebufferContent()
				{}

			FramebufferContent(FramebufferContent&& fbcontent):m_textures(std::move(fbcontent.m_textures))
				{}

			void framebufferResize(int width,int height)
				{
				for(decltype(size()) k=0;k<N;++k)
					{m_textures[k].realloc(width,height);}
				}

			template<size_t k>
			Angle::Texture2D& get() noexcept
				{return m_textures.get<k>();}

			size_t size() const noexcept
				{return m_textures.size();}

		private:
			static constexpr auto N=sizeof...(formats);
			ArrayFixed<Angle::Texture2D,N> m_textures;
		};
	}

#endif // GLINDE_FRAMEBUFFERCONTENT_HPP
