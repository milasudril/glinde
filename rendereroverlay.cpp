//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"rendereroverlay.o",
//@	            "type":"object"
//@	        }
//@	    ]
//@	}
#include "rendereroverlay.h"

#include "glprogram.h"
#include "glvertexarray.h"
#include "glvertexbuffer.h"
#include "glelementarraybuffer.h"
#include "gltexture.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

using namespace Glinde;

static const char* g_vert_shader="#version 330 core\n"
	"layout(location=0) in vec3 vertex_pos;"
	"layout(location=1) in vec2 uv_pos;"
	"out vec2 uv;"
	"void main()"
	"	{"
	"	gl_Position.xyz = vertex_pos;"
	"	gl_Position.w=1.0;"
	"	uv=uv_pos;"
	"	}";

static const char* g_frag_shader="#version 330 core\n"
	"in vec2 uv;"
	"uniform sampler2D image;"
	"out vec3 color;"
	"void main()"
	"	{"
	"	color=vec3(texture(image,uv));"
	"	}";

static const glm::vec3 g_verts[]
	{
	 {-1.0f,-1.0f,0.0f}
	,{ 1.0f,-1.0f,0.0f}
	,{ 1.0f, 1.0f,0.0f}
	,{-1.0f, 1.0f,0.0f}
	};

static const glm::vec2 g_uvs[]
	{
	 {0.0f,0.0f}
	,{1.0f,0.0f}
	,{1.0f,1.0f}
	,{0.0f,1.0f}
	};

static const uint32_t g_faces[]=
	{
	 0,1,2,2,3,0
	};

static size_t s_usecount=0;


namespace
	{
	class Resources
		{
		public:
			Resources();
			~Resources();

			void draw(const GlTexture& texture) noexcept
				{
				m_array.bind();
				m_program.use();
				glUniform1i(m_image_loc,0);
				texture.bind();
				m_faces.draw(0);
				}

		private:
			GlVertexArray m_array;
			GlVertexBuffer m_vbo;
			GlVertexBuffer m_uvs;
			GlElementArrayBuffer m_faces;
			GlProgram m_program;
			GLuint m_image_loc;
		};
	}

Resources::Resources()
	{
	GlShader vert_shader(GL_VERTEX_SHADER);
	vert_shader.sourceSet(g_vert_shader).compile();
	GlShader frag_shader(GL_FRAGMENT_SHADER);
	frag_shader.sourceSet(g_frag_shader).compile();

	m_program.shaderAttach(vert_shader).shaderAttach(frag_shader).link();
	m_program.shaderDetatch(frag_shader).shaderDetatch(vert_shader);

	m_array.bind();
	m_program.use();
	m_image_loc=m_program.uniformGet("image");
	m_vbo.dataSet(Range<const glm::vec3>(g_verts,4),GL_STATIC_DRAW);
	m_uvs.dataSet(Range<const glm::vec2>(g_uvs,4),GL_STATIC_DRAW);
	m_faces.dataSet(g_faces,6,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	m_vbo.attributesBind(0,3);
	m_uvs.attributesBind(1,2);
	}

Resources::~Resources()
	{
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	}


static Resources* rc;

RendererOverlay::RendererOverlay()
	{
	if(s_usecount==0)
		{
		rc=new Resources();
		}
	++s_usecount;
	}

RendererOverlay::~RendererOverlay()
	{
	--s_usecount;
	if(s_usecount==0)
		{delete rc;}
	}

void RendererOverlay::viewportSizeSet(int width,int height) noexcept
	{
	glViewport(0,0,width,height);
	}

void RendererOverlay::render(const GlTexture& texture) noexcept
	{
	rc->draw(texture);
	}
