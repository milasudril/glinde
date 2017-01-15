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

#include <GLFW/glfw3.h>


struct Layout
	{
	static constexpr Angle::VertexArray<Layout>::Attribute attributes[]=
		{
		 {3,Angle::ValueType::FLOAT}
		,{3,Angle::ValueType::FLOAT}
		};
	};

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

int main()
	{
	GLFWContext glfw(Angle::gl_version_requirements());
	try
		{
		Window mainwin;
		Angle::ContextGuard<Window> context(mainwin);
		auto version=Angle::init();
		printf("%s, %s, %s, %s\n"
			,version.vendor
			,version.renderer
			,version.version
			,version.glsl_version);

		while(!glfwWindowShouldClose(mainwin.handle()))
			{
			glfwPollEvents();
			glfwSwapBuffers(mainwin.handle());
			}

		}
	catch(const Angle::Error& err)
		{
		fprintf(stderr,"Error: %s\n",err.message());
		return -1;
		}

/*	Angle::Program prgm(Angle::Shader("",Angle::ShaderType::FRAGMENT_SHADER)
		,Angle::Shader("",Angle::ShaderType::VERTEX_SHADER));

	Angle::VertexArray<Layout> vao;
	decltype(vao)::AttribContextAll context(vao);*/
	return 0;
	}
