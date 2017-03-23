//@	{"targets":[{"name":"consolerenderer.o","type":"object"}]}

#include "consolerenderer.hpp"
#include "console.hpp"
#include "../color.hpp"
#include "../texture_upload.hpp"
#include "../time/timeinfo.hpp"

using namespace Glinde;

#ifdef __AVX2__
static Color color(uint8_t color_mask) noexcept
	{
	vec4_t<float> color;
	auto intensity=0.3333f*static_cast<float>( (color_mask & 8)/8 );
	vec4_t<uint32_t> masks{4u,2u,1u,0};
	masks&=color_mask;
	masks>>=vec4_t<uint32_t>{2,1,0,1};
	vec4_t<float> cvec=_mm_cvtepi32_ps(reinterpret_cast<__m128i>(masks));
	color=cvec*0.6667f + vec4_t<float>{intensity,intensity,intensity,intensity};
	// Adjust the green level
	color=(color_mask==6)?color*vec4_t<float>{1.0f,0.5,1.0f,1.0f}:color;
	// make it opaque 
	color[3]=1.0f;
	return Color::fromSRGB(color);
	}
#else
static Color color(uint8_t color_mask) noexcept
	{
	vec4_t<float> color;
	auto intensity=0.3333f*static_cast<float>( (color_mask & 8)/8 );
	vec4_t<uint32_t> masks{4u,2u,1u,0};
	masks&=color_mask;
	vec4_t<float> cvec=_mm_cvtepi32_ps(reinterpret_cast<__m128i>(masks));
	color=cvec*0.6667f*vec4_t<float>{0.25,0.5,1,1} + vec4_t<float>{intensity,intensity,intensity,intensity};
	// Adjust the green level
	color=(color_mask==6)?color*vec4_t<float>{1.0f,0.5,1.0f,1.0f}:color;
	// make it opaque 
	color[3]=1.0f;
	return Color::fromSRGB(color);
	}
#endif

static ConsoleRenderer::Colormap colors_generate() noexcept
	{
	ConsoleRenderer::Colormap ret;
	for(decltype(ret.length()) k=0;k<ret.length();++k)
		{ret[k]=color(k);}
	return ret;
	}

static const ConsoleRenderer::Colormap s_vgacolors=colors_generate();

ConsoleRenderer::ConsoleRenderer(const Image& charmap,const Console& con):r_con(&con)
,m_program(R"EOF(#version 430 core
layout(location=0) in vec4 vertex_pos;
layout(location=1) in uint colors;
layout(location=2) in vec2 uv_pos;

layout(location=2) uniform vec4 vertex_offset;
layout(location=3) uniform vec2 texture_size;
layout(location=4) uniform vec2 charcell_size;
layout(location=5) uniform vec4 colortab[16];

out vec4 frag_color_fg;
out vec4 frag_color_bg;
out vec2 uv;

void main()
	{
	gl_Position=vertex_pos + vertex_offset;
	frag_color_fg=colortab[colors&0xf];
	frag_color_bg=colortab[(colors&0xf0)>>4];
	uv=uv_pos*charcell_size/texture_size;
	}
)EOF"_vert,R"EOF(#version 430 core
layout(location=0) uniform sampler2D charmap;
layout(location=1) uniform float bg_opacity;

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
,m_charcells(4*con.sizeFull()),m_colors(4*con.sizeFull()),m_uvs(4*con.sizeFull())
,m_faces(3*2*con.sizeFull()),m_t_toggle(0),m_cursor_shown(0)
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
		.vertexBuffer<1>(m_colors)
		.vertexBuffer<2>(m_uvs)
		.enableVertexAttrib<0>()
		.enableVertexAttrib<1>()
		.enableVertexAttrib<2>()
		.elementBuffer(m_faces);

	m_program.bind();
	glUniform2f(3,charmap.width(),charmap.height());
	glUniform2f(4,CHARCELL_WIDTH,CHARCELL_HEIGHT);
	glUniform4fv(5,16,reinterpret_cast<const float*>(s_vgacolors.begin()));
	glUniform1f(1,1);
	m_program.unbind();
	}

constexpr Angle::VertexAttribute ConsoleRenderer::ShaderDescriptor::attributes[];

void ConsoleRenderer::render(Angle::Texture2D& texture,const Timeinfo& ti) const noexcept
	{
		{
		auto v=r_con->colorsFull();
		m_colors.bufferData(v.begin(),size_t(0),v.length());
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
		glUniform4f(2,0.0f,r_con->lineOffset(k),0.0f,0.0f);
		Angle::drawElements(Angle::DrawMode::TRIANGLES,((k + line_current)%n_rows) * n_cols,n_cols);
		}
	if(ti.simulationTime() - m_t_toggle >= 8.0/60.0 ) //Standard VGA blink rate
		{
		m_cursor_shown=!m_cursor_shown;
		m_t_toggle=ti.simulationTime();
		}

	if(m_cursor_shown)
		{
		auto pos=r_con->cursorPosition();
		glUniform4f(2,pos[0],pos[1],0.0f,0.0f);
		Angle::drawElements(Angle::DrawMode::TRIANGLES,n_cols*n_rows,6);
		}
	
	glEnable(GL_BLEND);
	
	texture.mipmapsGenerate()
		.filter(Angle::MagFilter::LINEAR)
		.filter(Angle::MinFilter::LINEAR_MIPMAP_LINEAR);
	}
