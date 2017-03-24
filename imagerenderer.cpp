//@	{
//@	"targets":[{"name":"imagerenderer.o","type":"object"}]
//@ }

#include "imagerenderer.hpp"
#include "instancecounter.hpp"
#include "angle/framebuffer.hpp"
#include "angle/vertexbuffer.hpp"
#include "angle/program.hpp"
#include "angle/vertexarray.hpp"
#include <geosimd/point.hpp>


using namespace Glinde;

namespace
	{
	struct ShaderLayout
		{
		static constexpr Angle::VertexAttribute attributes[]=
			{
			 {4,Angle::ConstantGet<float>::value}
			};
		};

	constexpr Angle::VertexAttribute ShaderLayout::attributes[];

	static constexpr GeoSIMD::Point<float> s_points[]=
		{
		 GeoSIMD::Point<float>(-1,-1,0)
		,GeoSIMD::Point<float>(1,-1,0)
		,GeoSIMD::Point<float>(1,1,0)
		,GeoSIMD::Point<float>(1,1,0)
		,GeoSIMD::Point<float>(-1,1,0)
		,GeoSIMD::Point<float>(-1,-1,0)
		};

	struct ImageRendererData
		{
		ImageRendererData():m_program(R"EOF(#version 430 core
layout(location=0) in vec4 position;
layout(location=0) uniform vec2 mirror;
out vec2 tex_coords;
void main()
	{
	gl_Position=position;
	tex_coords=position.xy*mirror;
	tex_coords=0.5*(tex_coords + vec2(1,1));
	}
)EOF"_vert,R"EOF(#version 430 core
out vec4 color;
in vec2 tex_coords;
layout(binding=0) uniform sampler2D texture_data;

void main()
	{
	color=texture(texture_data,tex_coords);
	}
)EOF"_frag),m_vertices(6)
			{
			m_vertices.bufferData(native_type(s_points),6);
			m_vao.vertexBuffer<0>(m_vertices).enableVertexAttrib<0>();
			}

		Angle::Program m_program;
		Angle::VertexBuffer<GeoSIMD::vec4_t<float>> m_vertices;
		Angle::VertexArray<ShaderLayout> m_vao;
		};
	}

static InstanceCounter<ImageRendererData> s_img_renderer_data;

ImageRenderer::ImageRenderer()
	{
	s_img_renderer_data.get<ImageRendererData>(ImageRendererData{});
	}

void ImageRenderer::render(const Angle::Texture2D& texture,bool mirror_x,bool mirror_y) const noexcept
	{
	auto& r=s_img_renderer_data.get();
	r.m_vao.bind();
	texture.bind<0>();
	r.m_program.uniform<0>(mirror_x?-1.0f:1.0f,mirror_y?-1.0f:1.0f).bind();
	glDrawArrays(GL_TRIANGLES,0,6);
	}

ImageRenderer::~ImageRenderer()
	{
	s_img_renderer_data.release();
	}
