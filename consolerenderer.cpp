//@	{"targets":[{"name":"consolerenderer.o","type":"object"}]}

#include "consolerenderer.hpp"
#include "console.hpp"
#include "texture_upload.hpp"

using namespace Glinde;

ConsoleRenderer::ConsoleRenderer(const Image& charmap):m_program(R"EOF(#version 450 core
layout(location=0) in vec4 position;
out vec2 tex_coords;
void main()
	{
	gl_Position=position;
	tex_coords=position.xy*vec2(1,-1);
	tex_coords=0.5*(tex_coords + vec2(1,1));
	}
)EOF"_vert,R"EOF(#version 450 core
out vec4 color;
in vec2 tex_coords;
layout(location=0) uniform sampler2D texture_data;

void main()
	{
	color=texture(texture_data,tex_coords);
	}
)EOF"_frag),m_charmap(texture2d(charmap))
	{}
