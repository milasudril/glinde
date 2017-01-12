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
#include "shader.hpp"

struct Layout
	{
	static constexpr Angle::VertexArray<Layout>::Attribute attributes[]=
		{
		 {3,Angle::ValueType::FLOAT}
		,{3,Angle::ValueType::FLOAT}
		};
	};


int main()
	{
	Angle::Shader frag_shader("",Angle::ShaderType::FRAGMENT_SHADER
		,[](auto type,const char* message)
			{
			throw "Shader compilation error";
			}
		);
	Angle::VertexArray<Layout> vao;
	decltype(vao)::AttribContextAll context(vao);
	return 0;
	}
