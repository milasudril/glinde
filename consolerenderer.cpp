//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"consolerenderer.o",
//@	            "type":"object"
//@	        }
//@	    ]
//@	}
#include "consolerenderer.h"
#include "console.h"

using namespace Glinde;

static const char* g_vert_shader="#version 330 core\n"
	"layout(location=0) in vec3 vertex_pos;"
	"layout(location=1) in vec3 color_fg;"
	"layout(location=2) in vec3 color_bg;"
	"layout(location=3) in vec2 uv_pos;"
	"uniform vec3 v_offset;"
	"out vec3 frag_color_fg;"
	"out vec3 frag_color_bg;"
	"out vec2 uv;"
	"void main()"
	"	{"
	"	gl_Position.xyz = vertex_pos + v_offset;"
	"	gl_Position.w=1.0;"
	"	frag_color_fg = color_fg;"
	"	frag_color_bg = color_bg;"
	"	uv=uv_pos;"
	"	}";

static const char* g_frag_shader="#version 330 core\n"
	"in vec3 frag_color_fg;"
	"in vec3 frag_color_bg;"
	"in vec2 uv;"
	"uniform sampler2D fontmap;"
	"layout(location = 0) out vec3 color;"
	"void main()"
	"	{"
	"	float fg=float( texture(fontmap,uv) );"
	"	color=frag_color_fg * fg + frag_color_bg*(1.0 - fg);"
	"	}";

ConsoleRenderer::ConsoleRenderer(const Console& console):r_console(&console)
	{
	GlShader vertex_shader(GL_VERTEX_SHADER);
	vertex_shader.sourceSet(g_vert_shader).compile();

	GlShader fragment_shader(GL_FRAGMENT_SHADER);
	fragment_shader.sourceSet(g_frag_shader).compile();

	m_program.shaderAttach(vertex_shader).shaderAttach(fragment_shader).link();
	m_program.shaderDetatch(fragment_shader).shaderDetatch(vertex_shader);

	m_array.bind();
	m_program.use();

	glClearColor(0.25,0.25,0.25,1);

	m_fontmap_loc=m_program.uniformGet("fontmap");
	m_voffset_loc=m_program.uniformGet("v_offset");
	m_fontmap.dataSet(console.fontmapGet());
	m_fontmap.filterDisable();

	auto width=static_cast<uint32_t>(r_console->windowWidthGet());
	auto height=static_cast<uint32_t>(r_console->windowHeightGet());
	m_fb.targetSet(m_result,width,height);

	m_vbo.dataSet(r_console->verticesGet(),GL_STATIC_DRAW);
	m_faces.dataSet(r_console->facesGet(),GL_STATIC_DRAW);
	m_uvs.dataSet(r_console->uvsGet(),GL_STATIC_DRAW);
	m_color_fg.dataSet(r_console->colorsFgGet(),GL_STATIC_DRAW);
	m_color_bg.dataSet(r_console->colorsBgGet(),GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	m_vbo.attributesBind(0,3);
	m_color_fg.attributesBind(1,3);
	m_color_bg.attributesBind(2,3);
	m_uvs.attributesBind(3,2);
	}

ConsoleRenderer::~ConsoleRenderer()
	{
	m_array.bind();
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	}

void ConsoleRenderer::render() noexcept
	{
	m_array.bind();
	m_fb.bind();
	glDisable(GL_DEPTH_TEST);
	m_fontmap.bind();
	m_program.use();
	glUniform1i(m_fontmap_loc,0);

	m_uvs.dataSet(r_console->uvsGet(),GL_STATIC_DRAW);
	m_color_fg.dataSet(r_console->colorsFgGet(),GL_STATIC_DRAW);
	m_color_bg.dataSet(r_console->colorsBgGet(),GL_STATIC_DRAW);

	auto N=r_console->rowsCountGet();
	for(size_t k=0;k<N;++k)
		{
		glUniform3f(m_voffset_loc,0.0f,r_console->lineOffsetGet(k),0.0f);
		m_faces.dataSet(r_console->facesGet(k),GL_STATIC_DRAW);
		m_faces.draw(0);
		}
	m_fb.unbind();
	m_result.filterEnable();
	}
