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
			ImageRenderer():m_vertices(6)
				,m_program(R"EOF(#version 450 core
layout(location=0) in vec4 position;
out vec2 tex_coords;
void main()
	{
	gl_Position=position;
	tex_coords=position.xy + vec2(0.5,0.5);
	}
)EOF"_vert,R"EOF(#version 450 core
out vec4 color;
in vec2 tex_coords;
layout(location=0) uniform sampler2D texture_data;

void main()
	{
	color=texture(texture_data,tex_coords);
	}
)EOF"_frag)
				{
				m_vertices.bufferData(native_type(s_points),6);
				m_vao.vertexBuffer<0>(m_vertices).enableVertexAttrib<0>();
				}

			void render(const Angle::Texture2D& texture,Angle::Framebuffer& fb) noexcept
				{
				m_vao.bind();
				texture.bind(0);
				m_program.bind();
				glUniform1i(0,0);
				glDrawArrays(GL_TRIANGLES,0,6);
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
