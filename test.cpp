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
	Angle::VertexArray<Layout> vao;
	decltype(vao)::AttribContext<0> context(vao);
	return 0;
	}
