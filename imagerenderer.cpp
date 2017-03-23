//@	{
//@	"targets":[{"name":"imagerenderer.o","type":"object"}]
//@ }

#include "imagerenderer.hpp"
using namespace Glinde;

constexpr Angle::VertexAttribute ImageRenderer::ShaderLayout::attributes[];
constexpr GeoSIMD::Point<float> ImageRenderer::s_points[];

ImageRenderer::ImageRenderer():m_vertices(6)
	,m_program(R"EOF(#version 430 core
layout(location=0) in vec4 position;
layout(location=1) uniform vec2 mirror;
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
layout(location=0) uniform sampler2D texture_data;

void main()
	{
	color=texture(texture_data,tex_coords);
	}
)EOF"_frag)
	{
	m_vertices.bufferData(native_type(s_points),6);
	m_vao.vertexBuffer<0>(m_vertices).enableVertexAttrib<0>();
	m_program.uniform<1>(1.0f,1.0f);
	}
