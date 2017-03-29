//@	{
//@	 "targets":[{"name":"scenerenderer.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"scenerenderer.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_SCENERENDERER_HPP
#define GLINDE_SCENERENDERER_HPP

#include "../angle/vertexarray.hpp"
#include "../angle/framebuffer.hpp"
#include "../angle/texture2d.hpp"

namespace Glinde
	{
	class Viewpoint;
	class Site;

	class SceneRenderer
		{
		public:
			SceneRenderer(int width,int height);
			~SceneRenderer() noexcept;

			void render(const Site& site,const Viewpoint& v) noexcept;

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
			Angle::Texture2D m_texture_out;
			Angle::Texture2D m_depthbuffer;
		};
	};

#endif
