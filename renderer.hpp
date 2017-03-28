//@	{
//@	 "targets":[{"name":"renderer.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"renderer.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_RENDERER_HPP
#define GLINDE_RENDERER_HPP

#include "angle/vertexarray.hpp"
#include "angle/framebuffer.hpp"

namespace Angle
	{class Texture2D;}

namespace Glinde
	{
	class Viewpoint;
	class Site;

	class Renderer
		{
		public:
			Renderer();
			~Renderer() noexcept;

			void render(const Site& mesh
				,const Viewpoint& v
				,Angle::Texture2D& texture_out) noexcept;

		private:
			struct ShaderDescriptor
				{
				static constexpr Angle::VertexAttribute attributes[]=
					{
					 {4,Angle::ConstantGet<float>::value}
					,{4,Angle::ConstantGet<float>::value}
					,{2,Angle::ConstantGet<float>::value}
					};
				};

			Angle::VertexArray<ShaderDescriptor> m_vao;
			Angle::Framebuffer m_fb;
		};
	};

#endif
