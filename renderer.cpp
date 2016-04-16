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

#define GLM_FORCE_RADIANS

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

static const char* g_frag_shader="#version 450 core\n"
	"out vec3 color;"
	"in vec2 UV;"
	"in vec3 pos_worldspace;"
	"in vec3 normal_cameraspace;"
	"in vec3 eye_dir_cameraspace;"
	"in vec3 light_dir_cameraspace;"
	"uniform sampler2D texture_diffuse;"
	"uniform vec3 lightpos_worldspace;"
	"void main()"
	"	{"
	"	vec3 light_color=vec3(1,1,1);"
	"	float power=1.0f;"
	"	float d=length(lightpos_worldspace - pos_worldspace);"
	"	vec3 n=normalize(normal_cameraspace);"
	"	vec3 l=normalize(light_dir_cameraspace);"
	"	float dotprod=clamp(dot(n,l),0,1);"
/*	"	vec3 e=normalize(eye_dir_cameraspace);"
	"	vec3 r=reflect(-l,n);"*/
	"	color=texture(texture_diffuse, UV).rgb*light_color*dotprod/(d*d);"
	"	}";

static const char* g_vert_shader=
	"#version 450 core\n"
	"layout(location = 0) in vec3 pos_modelspace;"
	"layout(location = 1) in vec3 normal_modelspace;"
	"layout(location = 2) in vec2 uv;"
	"out vec3 pos_worldspace;"
	"out vec3 normal_cameraspace;"
	"out vec3 eye_dir_cameraspace;"
	"out vec3 light_dir_cameraspace;"
	"out vec2 UV;"
	"uniform mat4 MVP;"
	"uniform mat4 V;"
	"uniform mat4 M;"
	"uniform vec3 lightpos_worldspace;"
	"void main()"
	"	{"
	"	gl_Position=MVP*vec4(pos_modelspace,1);"
	"	pos_worldspace=(vec4(pos_modelspace,1)).xyz;"
	"	vec3 pos_cameraspace=(V*M*vec4(pos_modelspace,1)).xyz;"
	"	eye_dir_cameraspace=vec3(0,0,0) - pos_cameraspace;"
	"	vec3 lightpos_cameraspace=(V*vec4(lightpos_worldspace,1)).xyz;"
	"	light_dir_cameraspace=lightpos_cameraspace + eye_dir_cameraspace;"
	"	normal_cameraspace=(V*M*vec4(normal_modelspace,0)).xyz;"
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
	V_id=program.uniformGet("V");
	M_id=program.uniformGet("M");
	lightpos_id=program.uniformGet("lightpos_worldspace");
	diffuse_id=program.uniformGet("texture_diffuse");

	glUniform3f(lightpos_id,0.0f,1.0f,0.75f);

	glEnable(GL_DEPTH_TEST|GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	}

Renderer::~Renderer()
	{}

void Renderer::sceneRender(World& world,const WorldObject& viewer) noexcept
	{
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	auto V=glm::lookAt(glm::vec3(0,0,2),glm::vec3(0,0,0),glm::vec3(0,1,0));
	glUniformMatrix4fv(V_id,1,GL_FALSE,&V[0][0]);
	auto V1=viewer.viewMatrixGet();
	auto VP=P*V1;
	auto MVP=VP;
	glm::mat4 M;
	glUniformMatrix4fv(M_id,1,GL_FALSE,&M[0][0]);
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

			normals.dataSet(frame.m_normals.begin()
				,static_cast<unsigned int>(frame.m_normals.length())
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
			normals.attributesBind(1,3);
			uvs.attributesBind(2,2);
			texture.dataSet(*frame.r_tex_diffuse);
			vertex_indices.draw(0);
			++ptr;
			}
		}
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	}

void Renderer::viewportSizeSet(int width,int height) noexcept
	{
	GLINDA_DEBUG_PRINT("Viewport size changed to %d x %d",width,height);
	glViewport(0,0,width,height);
	P=glm::perspective(2.0f*std::acos(0.0f)/3.0f,float(width)/float(height),0.1f,1000.0f);
	}
