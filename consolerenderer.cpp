//@	{"targets":[{"name":"consolerenderer.o","type":"object"}]}

#include "consolerenderer.hpp"
#include "console.hpp"
#include "texture_upload.hpp"

using namespace Glinde;

ConsoleRenderer::ConsoleRenderer(const Image& charmap,const Console& con):r_con(&con)
,m_program(R"EOF(#version 450 core
layout(location=0) in vec4 vertex_pos;
layout(location=1) in vec4 color_fg;
layout(location=2) in vec4 color_bg;
layout(location=3) in vec2 uv_pos;
layout(location=4) uniform vec4 vertex_offset;
layout(location=5) uniform vec2 texture_size;
layout(location=6) uniform vec2 charcell_size;

out vec4 frag_color_fg;
out vec4 frag_color_bg;
out vec2 uv;

void main()
	{
	gl_Position=vertex_pos + vertex_offset;
	frag_color_fg=color_fg;
	frag_color_bg=color_bg;
	uv=uv_pos*charcell_size/texture_size;
	}
)EOF"_vert,R"EOF(#version 450 core
layout(location=0) uniform sampler2D charmap;
layout(location=7) uniform float bg_opacity;

in vec2 uv;
in vec4 frag_color_fg;
in vec4 frag_color_bg;
layout(location=0) out vec4 color;

void main()
	{
	float fg=float(texture(charmap,uv));
	color=frag_color_fg*fg + frag_color_bg*(1.0 - fg)*bg_opacity;
	}
)EOF"_frag),m_charmap(texture2d(charmap,1))
,m_charcells(4*con.sizeFull()),m_fg(4*con.sizeFull()),m_bg(4*con.sizeFull()),m_uvs(4*con.sizeFull())
,m_faces(3*2*con.sizeFull())
	{
	m_charmap.filter(Angle::MagFilter::NEAREST)
		.filter(Angle::MinFilter::NEAREST);
	auto v=con.verticesFull();
	m_charcells.bufferData(native_type(v.begin()),v.length());
	auto f=con.facesFull();
	m_faces.bufferData(native_type(f.begin()),3*f.length());

	auto uvs=con.uvsFull();
	m_uvs.bufferData(uvs.begin(),uvs.length());

	m_vao.vertexBuffer<0>(m_charcells)
		.vertexBuffer<1>(m_fg)
		.vertexBuffer<2>(m_bg)
		.vertexBuffer<3>(m_uvs)
		.enableVertexAttrib<0>()
		.enableVertexAttrib<1>()
		.enableVertexAttrib<2>()
		.enableVertexAttrib<3>()
		.elementBuffer(m_faces);

	m_program.bind();
	glUniform2f(5,charmap.width(),charmap.height());
	glUniform2f(6,CHARCELL_WIDTH,CHARCELL_HEIGHT);
//	glUniform1f(7,1-1.0f/16.0f);
	glUniform1f(7,1);
	m_program.unbind();
	}

constexpr Angle::VertexAttribute ConsoleRenderer::ShaderDescriptor::attributes[];

void ConsoleRenderer::render(Angle::Texture2D& texture) const noexcept
	{
		{
		auto v=r_con->colorsBgFull();
		m_bg.bufferData(native_type(v.begin()),size_t(0),v.length());

		v=r_con->colorsFgFull();
		m_fg.bufferData(native_type(v.begin()),size_t(0),v.length());
		}
		{
		auto v=r_con->uvs();
		m_uvs.bufferData(v.begin(),size_t(0),v.length());
		}

	texture.filter(Angle::MagFilter::NEAREST)
		.filter(Angle::MinFilter::NEAREST);
	m_vao.bind();
	m_charmap.bind(0);
	m_program.bind();
	auto n_rows=r_con->rowsCount();
	auto n_cols=r_con->colsCount();

	m_fb.attachColorBuffer<0>(texture,0)
		.colorBuffersOutputActivate(0u);
	m_fb.bind(Angle::Framebuffer::Target::DRAW);
	glViewport(0,0,textureWidth(),textureHeight());
	glDisable(GL_DEPTH_TEST);

	glDisable(GL_BLEND);
	n_cols*=6; //3*2 vertices per faces
	auto line_current=r_con->lineCurrent();
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
	for(decltype(n_rows) k=0;k<n_rows;++k)
		{
		glUniform4f(4,0.0f,r_con->lineOffset(k),0.0f,0.0f);
		Angle::drawElements(Angle::DrawMode::TRIANGLES,((k + line_current)%n_rows) * n_cols,n_cols);
		}
	auto pos=r_con->cursorPosition();
	glUniform4f(4,pos[0],pos[1],0.0f,0.0f);
	Angle::drawElements(Angle::DrawMode::TRIANGLES,n_cols*n_rows,6);
	glEnable(GL_BLEND);
	

	texture.mipmapsGenerate()
		.filter(Angle::MagFilter::LINEAR)
		.filter(Angle::MinFilter::LINEAR_MIPMAP_LINEAR);
	}
