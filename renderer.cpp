//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[
//@	                {
//@	                    "ref":"GLEW",
//@	                    "rel":"external"
//@	                }
//@	            ],
//@	            "name":"renderer.o",
//@	            "type":"object"
//@	        }
//@	    ]
//@	}
#define GLM_FORCE_RADIANS

#include "renderer.h"
#include "glshader.h"
#include "worldobject.h"
#include "sitedefault.h"
#include "debug.h"
#include "range.h"
#include "model.h"
#include "image.h"

using namespace Glinde;

static const char* g_frag_shader="#version 330 core\n"
	"out vec3 color;"
	"in vec2 UV;"
	"in vec3 pos_worldspace;"
	"in vec3 normal_cameraspace;"
	"in vec3 eye_dir_cameraspace;"
	"in vec3 light_dir_cameraspace;"
	"uniform sampler2D texture_diffuse;"
	"uniform vec3 lightpos_worldspace;"
	""
	"float tosRGB(float x)"
	"	{"
	"	return x<=0.0031308f? 12.92f*x : (1 + 0.055f)*pow(x,1/2.4f) - 0.055f;"
	"	}"
	""
	"vec3 tosRGB(vec3 x)"
	"	{"
	"	return vec3(tosRGB(x.r),tosRGB(x.g),tosRGB(x.b));"
	"	}"
	""
	"void main()"
	"	{"
	"	vec3 light_color=vec3(1,0.875,0.75);"
	"	float power=1.0f;"
	"	float d=length(lightpos_worldspace - pos_worldspace);"
	"	vec3 n=normalize(normal_cameraspace);"
	"	vec3 l=normalize(light_dir_cameraspace);"
	"	float dotprod=clamp(dot(n,l),0,1);"
/*	"	vec3 e=normalize(eye_dir_cameraspace);"
	"	vec3 r=reflect(-l,n);"*/
	"	color=tosRGB(texture(texture_diffuse, UV).rgb*light_color*dotprod/(d*d));"
	"	}";

static const char* g_vert_shader=
	"#version 330 core\n"
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

	glUniform3f(lightpos_id,0.0f,0.0f,2.4f);

	glClearColor(0.25,0.25,0.25,1);
	vertices.attributesBind(0,3);
	normals.attributesBind(1,3);
	uvs.attributesBind(2,2);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	}

Renderer::~Renderer()
	{
	array.bind();
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	}

void Renderer::render(const Range< const Mesh >& meshes) noexcept
	{
	auto mesh=meshes.begin();
	auto mesh_end=meshes.end();

	while(mesh!=mesh_end)
		{
		vertices.dataSet(mesh->verticesGet(),GL_STATIC_DRAW);
		normals.dataSet(mesh->normalsGet(),GL_STATIC_DRAW);
		uvs.dataSet(mesh->uvsGet(),GL_STATIC_DRAW);


		vertex_indices.dataSet(mesh->facesIndirectGet(),GL_STATIC_DRAW);
			{
			auto& texture=**(mesh->texturesGet().begin());
			auto id=texture.idGet();
			auto tex=textures.resourceGet(id);
			if(!tex.second)
				{
				GLINDE_DEBUG_PRINT("Cache miss for texture #0;",id);
				tex.first.tagSet(id).objectGet().dataSet(texture);
				}
			tex.first.objectGet().bind();
			}
		vertex_indices.draw(0);

		++mesh;
		}
	}

void Renderer::sceneRender(const SiteDefault& site,const WorldObject& viewer) noexcept
	{
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	array.bind();
	program.use();
//	auto V=glm::lookAt(glm::vec3(0,-2,1.7f),glm::vec3(0,0,1.55),glm::vec3(0,0,1));
//	auto V=glm::lookAt(glm::vec3(0,0,2.5f),glm::vec3(0,0,0),glm::vec3(0,1,0));
	auto V=viewer.viewMatrixGet();
	glUniformMatrix4fv(V_id,1,GL_FALSE,&V[0][0]);
	auto VP=P*V;
	glUniform1i(diffuse_id,0);

		{
		glm::mat4 M;
		glUniformMatrix4fv(M_id,1,GL_FALSE,&M[0][0]);
		glUniformMatrix4fv(MVP_id,1,GL_FALSE,&VP[0][0]);
		render(site.modelGet().frameGet(0).meshes);
		}

		{
		auto objects=site.objectsGet();
		auto ptr=objects.begin();
		auto ptr_end=objects.end();
		while(ptr!=ptr_end)
			{
			auto& obj=ptr->object();
			if(obj.modelGet()!=nullptr)
				{
				glm::mat4 M=obj.modelMatrixGet();
				auto MVP=VP*M;
				glUniformMatrix4fv(M_id,1,GL_FALSE,&M[0][0]);
				glUniformMatrix4fv(MVP_id,1,GL_FALSE,&MVP[0][0]);
				render(obj.frameCurrentGet().meshes);
				}
			++ptr;
			}
		}
	}

void Renderer::viewportSizeSet(int width,int height) noexcept
	{
	GLINDE_DEBUG_PRINT("Viewport size changed to #0; x #1;",width,height);
	glViewport(0,0,width,height);
	P=glm::perspective(2.5f*std::acos(0.0f)/3.0f,float(width)/float(height),0.1f,1000.0f);
	}
