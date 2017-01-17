//@ {
//@	 "targets":
//@		[
//@			{
//@			 "name":"test","type":"application"
//@			,"pkgconfig_libs":["glfw3"]
//@			}
//@		]
//@	}

#include "vertexarray.hpp"
#include "program.hpp"
#include "init.hpp"
#include "contextguard.hpp"

#include <geosimd/point.hpp>
#include <GLFW/glfw3.h>

struct GLFWContext
	{
	GLFWContext(const Angle::VersionRequest& version)
		{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version.major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version.minor);
		switch(version.profile)
			{
			case Angle::VersionRequest::Profile::CORE:
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
				break;
			case Angle::VersionRequest::Profile::COMPAT:
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
				break;
			case Angle::VersionRequest::Profile::ANY:
			default:
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
				break;
			}
		if(version.forward_compatible)
			{glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);}
		}
	~GLFWContext()
		{glfwTerminate();}
	};

class Window
	{
	public:
		typedef GLFWwindow* ContextHandle;

		Window(const Window&)=delete;
		Window& operator=(const Window&)=delete;

		Window()
			{m_handle=glfwCreateWindow(800,600,"",nullptr,nullptr);}

		~Window()
			{glfwDestroyWindow(m_handle);}

		auto contextCapture() noexcept
			{
			auto ret=glfwGetCurrentContext();
			glfwMakeContextCurrent(m_handle);
			return ret;
			}

		static void contextRelease(GLFWwindow* window) noexcept
			{glfwMakeContextCurrent(window);}

		auto handle() noexcept
			{return m_handle;}

		bool shouldClose() const noexcept
			{return glfwWindowShouldClose(m_handle);}

	private:
		GLFWwindow* m_handle;
	};

static constexpr GeoSIMD::Point<float> verts[]=
	{
	 GeoSIMD::Point<float>{0.5f,  0.5f, 0.0f}
	,GeoSIMD::Point<float>{0.5f, -0.5f, 0.0f}
	,GeoSIMD::Point<float>{-0.5f, -0.5f, 0.0f}
	,GeoSIMD::Point<float>{-0.5f,  0.5f, 0.0f}
	};

static constexpr uint16_t faces[]=
	{
	 0,1,3
	,1,2,3
	};

template<class T>
static constexpr const GeoSIMD::vec4_t<T>* native_type(const GeoSIMD::Point<T>* point_ptr)
	{return reinterpret_cast<const GeoSIMD::vec4_t<T>*>(point_ptr);}

struct MyShaderLayout
	{
	static constexpr Angle::VertexAttribute attributes[]=
		{
			{4,Angle::ConstantGet<float>::value}
		};
	};

constexpr Angle::VertexAttribute MyShaderLayout::attributes[];

#ifndef NDEBUG
static void APIENTRY openglCallbackFunction(
  GLenum source,
  GLenum type,
  GLuint id,
  GLenum severity,
  GLsizei length,
  const GLchar* message,
  const void* userParam
){
  (void)source; (void)type; (void)id;
  (void)severity; (void)length; (void)userParam;
  fprintf(stderr, "%s\n", message);
}
#endif

int main()
	{
	GLFWContext glfw(Angle::gl_version_requirements());
	try
		{
		Window mainwin;
		Angle::ContextGuard<Window> context(mainwin);
		auto version=Angle::init();
#ifndef NDEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(openglCallbackFunction, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
#endif

		printf("%s, %s, %s, %s\n"
			,version.vendor
			,version.renderer
			,version.version
			,version.glsl_version);

		Angle::VertexBuffer<GeoSIMD::vec4_t<float>>	vertbuff(4);
		vertbuff.bufferData(native_type(verts),4);
		Angle::VertexBuffer<uint16_t> facebuff(6);
		facebuff.bufferData(faces,6);

		Angle::Program prgm(
R"EOF(#version 450 core
layout(location=0) in vec4 position;
void main()
	{
	gl_Position=position;
	}
)EOF"_vert,R"EOF(#version 450 core
out vec4 color;
layout(location=0) uniform vec4 color_in;

void main()
	{
	color=color_in;
	}
)EOF"_frag);

		Angle::VertexArray<MyShaderLayout> vertex_array;
		vertex_array.vertexBuffer<0>(vertbuff).enableVertexAttrib<0>()
			.elementBuffer(facebuff);

		while(!mainwin.shouldClose())
			{
			glfwPollEvents();
			vertex_array.bind();
			prgm.bind();
			glUniform4f(0,39.2e-2, 56.1e-2, 95.3e-2,1.0f);
			Angle::drawElements(Angle::DrawMode::TRIANGLES,0,3);
			glUniform4f(0,0.0f,0.3f,0.0f,1.0f);
			Angle::drawElements(Angle::DrawMode::TRIANGLES,3,3);
			glfwSwapBuffers(mainwin.handle());
			}

		}
	catch(const Angle::Error& err)
		{
		fprintf(stderr,"Error: %s\n",err.message());
		return -1;
		}
	return 0;
	}
