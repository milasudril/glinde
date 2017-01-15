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
	Angle::Program prgm(Angle::Shader("",Angle::ShaderType::FRAGMENT_SHADER)
		,Angle::Shader("",Angle::ShaderType::VERTEX_SHADER));

	Angle::VertexArray<Layout> vao;
	decltype(vao)::AttribContextAll context(vao);
	return 0;
	}
