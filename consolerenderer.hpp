//@	{
//@	 "dependencies_extra":[{"ref":"consolerenderer.o","rel":"implementation"}]
//@	,"targets":[{"name":"consolerenderer.hpp","type":"include"}]
//@	}

#ifndef GLINDE_CONSOLERENDERER_HPP
#define GLINDE_CONSOLERENDERER_HPP

#include "vectortype.hpp"
#include "faceindirect.hpp"
#include "renderobject.hpp"
#include "angle/texture2d.hpp"
#include "angle/program.hpp"
#include "angle/vertexbuffer.hpp"
#include "angle/vertexarray.hpp"

namespace Glinde
	{
	class Console;
	class Image;
	class ConsoleRenderer final:public RenderObject
		{
		public:
			ConsoleRenderer(const Image& charmap,Console&&)=delete;
			ConsoleRenderer(const Image& charmap,const Console& con);

			void render(Display& disp) const noexcept;

			void framebufferResize(int width,int height)
				{}

		private:
			const Console* r_con;
			Angle::Program m_program;
			Angle::Texture2D m_charmap;

			Angle::VertexBuffer<vec4_t<float>> m_charcells;
			mutable Angle::VertexBuffer<vec4_t<float>,Angle::BufferUsage::DYNAMIC_DRAW> m_fg;
			mutable Angle::VertexBuffer<vec4_t<float>,Angle::BufferUsage::DYNAMIC_DRAW> m_bg;
			mutable Angle::VertexBuffer<vec2_t<float>,Angle::BufferUsage::DYNAMIC_DRAW> m_uvs;
			Angle::VertexBuffer<uint16_t> m_faces;

			struct ShaderDescriptor
				{
				static constexpr Angle::VertexAttribute attributes[]=
					{
					 {decltype(m_charcells)::components,Angle::ConstantGet<float>::value}
					,{decltype(m_fg)::components,Angle::ConstantGet<float>::value}
					,{decltype(m_bg)::components,Angle::ConstantGet<float>::value}
					,{decltype(m_uvs)::components,Angle::ConstantGet<float>::value}
					};
				};

			Angle::VertexArray<ShaderDescriptor> m_vao;
		};
	}

#endif
