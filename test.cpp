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
	GLFWContext(const Angle::Version& version)
		{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version.major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version.minor);
		switch(version.profile)
			{
			case Angle::Version::Profile::CORE:
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
				break;
			case Angle::Version::Profile::COMPAT:
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
				break;
			case Angle::Version::Profile::ANY:
			default:
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
				break;
			}
		
		}
	~GLFWContext()
		{glfwTerminate();}
	};

int main()
	{
	GLFWContext glfw(Angle::version_requirements);
	try
		{
	//	Angle::init(WindowFactory{});
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
