//@ {
//@	 "targets":
//@		[
//@			{
//@			 "name":"test","type":"application"
//@			,"pkgconfig_libs":["glfw3","glew"]
//@			}
//@		]
//@	}

#include "vertexarray.hpp"
#include "program.hpp"
#include "init.hpp"
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

	private:
		GLFWwindow* m_handle;
	};

static constexpr GeoSIMD::Point<float> triangle[]
	{
	 GeoSIMD::Point<float>{-0.5f, -0.5f, 0.0f}
	,GeoSIMD::Point<float>{0.5f, -0.5f, 0.0f}
	,GeoSIMD::Point<float>{0.0f, 0.5f, 0.0f}
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

		Angle::VertexBuffer<GeoSIMD::vec4_t<float>>	vertices(3);
		vertices.bufferData(native_type(triangle),3);

		Angle::Program prgm(
R"EOF(#version 450 core
layout(location=0) in vec4 position;
void main()
	{
	gl_Position=position;
	}
)EOF"_vert,R"EOF(#version 450 core
out vec4 color;
void main()
	{
	color=vec4(1.0f,0.5f,0.2f,1.0f);
	}
)EOF"_frag);

		Angle::VertexArray<MyShaderLayout> vertex_array;
		vertex_array.vertexBuffer<0>(vertices).enableVertexAttrib<0>();

		while(!glfwWindowShouldClose(mainwin.handle()))
			{
			glfwPollEvents();
			vertex_array.bind();
			prgm.bind();
			glDrawArrays(GL_TRIANGLES,0,3);
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
