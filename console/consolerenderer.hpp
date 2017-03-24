//@	{
//@	 "dependencies_extra":[{"ref":"consolerenderer.o","rel":"implementation"}]
//@	,"targets":[{"name":"consolerenderer.hpp","type":"include"}]
//@	}

#ifndef GLINDE_CONSOLERENDERER_HPP
#define GLINDE_CONSOLERENDERER_HPP

#include "console.hpp"
#include "../instancecounter.hpp"
#include "../angle/texture2d.hpp"
#include "../angle/program.hpp"
#include "../angle/vertexbuffer.hpp"
#include "../angle/vertexarray.hpp"
#include "../angle/framebuffer.hpp"

namespace Glinde
	{
	class Image;
	class Timeinfo;
	class ConsoleRenderer
		{
		public:
			typedef ArrayFixed<Color,16> Colormap;

			static constexpr auto CHARCELL_WIDTH=9.0f;
			static constexpr auto CHARCELL_HEIGHT=16.0f;

			ConsoleRenderer(const Image& charmap,Console&&)=delete;
			explicit ConsoleRenderer(const Image& charmap,const Console& con);
			~ConsoleRenderer();

			void render(Angle::Texture2D& tex,const Timeinfo& ti) const noexcept;

			int textureWidth() const noexcept
				{return r_con->colsCount()*CHARCELL_WIDTH;}

			int textureHeight() const noexcept
				{return r_con->rowsCount()*CHARCELL_HEIGHT;}

			float bgOpacity() const noexcept
				{return m_bg_opacity;}

			ConsoleRenderer& bgOpacity(float v) noexcept
				{
				m_bg_opacity=v;
				return *this;
				}

		private:
			const Console* r_con;

			Angle::VertexBuffer<vec4_t<float>> m_palette;
			float m_bg_opacity;



			Angle::Texture2D m_charmap;
			Angle::VertexBuffer<vec4_t<float>> m_charcells;
			mutable Angle::VertexBuffer<uint32_t,Angle::BufferUsage::DYNAMIC_DRAW> m_colors;
			mutable Angle::VertexBuffer<vec2_t<float>,Angle::BufferUsage::DYNAMIC_DRAW> m_uvs;
			Angle::VertexBuffer<uint16_t> m_faces;

			mutable Angle::Framebuffer m_fb;
			mutable double m_t_toggle;
			mutable bool m_cursor_shown;

			struct ShaderDescriptor
				{
				static constexpr Angle::VertexAttribute attributes[]=
					{
					 {decltype(m_charcells)::components,Angle::ConstantGet<float>::value}
					,{decltype(m_colors)::components,Angle::ConstantGet<uint32_t>::value}
					,{decltype(m_uvs)::components,Angle::ConstantGet<float>::value}
					};
				};

			Angle::VertexArray<ShaderDescriptor> m_vao;
		};
	}

#endif
