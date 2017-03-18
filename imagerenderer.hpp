//@	{
//@	"targets":[{"name":"imagerenderer.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"imagerenderer.o","rel":"implementation"}]
//@ }

#ifndef GLINDE_IMAGERENDERER_HPP
#define GLINDE_IMAGERENDERER_HPP

#include "angle/framebuffer.hpp"
#include "angle/vertexbuffer.hpp"
#include "angle/program.hpp"
#include "angle/vertexarray.hpp"
#include <geosimd/point.hpp>

namespace Glinde
	{
	class ImageRenderer
		{
		public:
			ImageRenderer();
			~ImageRenderer()
				{
				m_vao.disableVertexAttrib<0>()
					.unbind();
				}

			void render(const Angle::Texture2D& texture) const noexcept
				{
				m_vao.bind();
				texture.bind(0);
				m_program.bind();
				glDrawArrays(GL_TRIANGLES,0,6);
				}

			ImageRenderer& mirror(bool x,bool y) noexcept
				{
				m_program.bind();
				glUniform2f(1
					,x?-1.0f:1.0f
					,y?-1.0f:1.0f);
				m_program.unbind();
				return *this;
				}
			
		private:
			struct ShaderLayout
				{
				static constexpr Angle::VertexAttribute attributes[]=
					{
					 {4,Angle::ConstantGet<float>::value}
					};
				};

			static constexpr GeoSIMD::Point<float> s_points[]=
				{
				 GeoSIMD::Point<float>(-1,-1,0)
				,GeoSIMD::Point<float>(1,-1,0)
				,GeoSIMD::Point<float>(1,1,0)
				,GeoSIMD::Point<float>(1,1,0)
				,GeoSIMD::Point<float>(-1,1,0)
				,GeoSIMD::Point<float>(-1,-1,0)
				};

			Angle::VertexArray< ShaderLayout > m_vao;
			Angle::VertexBuffer< GeoSIMD::vec4_t<float> > m_vertices;
			Angle::Program m_program;
		};
	}

#endif
