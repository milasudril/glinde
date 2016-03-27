#ifdef __WAND__
target
	[
	name[renderer.o]
	type[object]
	dependnecy[GL;external]
	dependency[GLEW;external]
	]
#endif

#include "renderer.h"
#include "glshader.h"
#include "worldobject.h"
#include "debug.h"

using namespace Glinda;

static const GLfloat g_vertex_buffer_data[]=
	{
	-1.0f, 0.0f,0.0f,
	1.0f, 0.0f,0.0f,
	0.0f, 0.0f, 1.0f
	};

static const char* g_frag_shader="#version 330 core\n"
	"out vec3 color;"
	"in vec2 UV;"
	"uniform sampler2D texture_diffuse;"
	"void main()"
	"	{"
	"	color=texture(texture_diffuse, UV).rgb;"
	"	}";

static const char* g_vert_shader=
	"#version 330 core\n"
	"layout(location = 0) in vec3 pos_modelspace;"
	"layout(location = 1) in vec2 uv;"
	"out vec2 UV;"
	"uniform mat4 MVP;"
	"void main()"
	"	{"
	"	gl_Position=MVP*vec4(pos_modelspace,1);"
	"	UV=uv;"
	"	}";


Renderer::Renderer()
	{
	GlShader vertex_shader(GL_VERTEX_SHADER);
	vertex_shader.sourceSet(g_vert_shader).compile();

	GlShader fragment_shader(GL_FRAGMENT_SHADER);
	fragment_shader.sourceSet(g_frag_shader).compile();

	program.shaderAttach(vertex_shader).shaderAttach(fragment_shader).link();

	program.shaderDetatch(fragment_shader).shaderDetatch(vertex_shader);

	array.bind();
	vbo.dataSet(sizeof(g_vertex_buffer_data),g_vertex_buffer_data,GL_STATIC_DRAW);
	program.use();
	MVP_id=program.uniformGet("MVP");
	}

Renderer::~Renderer()
	{}

void Renderer::sceneRender(World& world,WorldObject& camera) noexcept
	{
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnableVertexAttribArray(0);
	auto V=glm::lookAt(glm::vec3(0,0,2),glm::vec3(0,0,0),glm::vec3(0,1,0));
	auto V1=camera.viewMatrixGet();
	auto VP=P*V1;
	auto MVP=VP;
	glUniformMatrix4fv(MVP_id,1,GL_FALSE,&MVP[0][0]);
	vbo.draw(0);
	glDisableVertexAttribArray(0);
	}

void Renderer::viewportSizeSet(int width,int height) noexcept
	{
	GLINDA_DEBUG_PRINT("Viewport size changed to %d x %d",width,height);
	glViewport(0,0,width,height);
	P=glm::perspective(std::acos(0.0f),float(width)/height,0.1f,1000.0f);
	}
