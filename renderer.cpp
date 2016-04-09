#ifdef __WAND__
target
	[
	name[renderer.o]
	type[object]
	dependnecy[GL;external]
	dependency[GLEW;external]
	]
#endif

#define NDEBUG

#include "renderer.h"
#include "glshader.h"
#include "worldobject.h"
#include "world.h"
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
//	"	color.rgb=vec3(UV.x,UV.y,0.0);"
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
	program.use();
	MVP_id=program.uniformGet("MVP");
	diffuse_id=program.uniformGet("texture_diffuse");
	GLINDA_DEBUG_PRINT("diffuse uniform is %u",diffuse_id);
	glEnable(GL_DEPTH_TEST|GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);
	glFrontFace(GL_CCW);
	}

Renderer::~Renderer()
	{}

void Renderer::sceneRender(World& world,WorldObject& camera) noexcept
	{
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	auto V=glm::lookAt(glm::vec3(0,0,2),glm::vec3(0,0,0),glm::vec3(0,1,0));
	auto V1=camera.viewMatrixGet();
	auto VP=P*V1;
	auto MVP=VP;
	glUniformMatrix4fv(MVP_id,1,GL_FALSE,&MVP[0][0]);
	glUniform1i(diffuse_id,0);

		{
		auto ptr=world.objectsBegin();
		auto ptr_end=world.objectsEnd();
		while(ptr!=ptr_end)
			{
			auto mesh=ptr->meshGet();
			auto& frame=mesh->m_frames[0];
			vertices.dataSet(frame.m_vertices.begin()
				,static_cast<unsigned int>(frame.m_vertices.length())
				,GL_STATIC_DRAW);

			GLINDA_DEBUG_PRINT("vertex: %u   UV: %u"
				,frame.m_vertices.length(),frame.m_uv.length());

			uvs.dataSet(frame.m_uv.begin()
				,static_cast<unsigned int>(frame.m_uv.length())
				,GL_STATIC_DRAW);

			vertex_indices.dataSet(mesh->m_faces.begin()
				,static_cast<unsigned int>(mesh->m_faces.length())
				,GL_STATIC_DRAW);
			vertices.attributesBind(0,3);
			uvs.attributesBind(1,2);
			texture.dataSet(*frame.r_tex_diffuse);
			vertex_indices.draw(0);
			++ptr;
			}
		}
	glEnableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	}

void Renderer::viewportSizeSet(int width,int height) noexcept
	{
	GLINDA_DEBUG_PRINT("Viewport size changed to %d x %d",width,height);
	glViewport(0,0,width,height);
	P=glm::perspective(std::acos(0.0f),float(width)/float(height),0.1f,1000.0f);
	}
