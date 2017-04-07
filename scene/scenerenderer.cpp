/*
Glinde
Copyright (C) 2017  Torbjörn Rathsman

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/
//@	{
//@	 "targets":[{"name":"scenerenderer.o","type":"object"}]
//@	}

#include "meshhosted.hpp"
#include "scenerenderer.hpp"
#include "viewpoint.hpp"
#include "../instancecounter.hpp"
#include "../angle/program.hpp"

using namespace Glinde;

static InstanceCounter<Angle::Program> s_program;

SceneRenderer::SceneRenderer()
	{
	s_program.get<Angle::Shader,Angle::Shader>(R"EOF(#version 430 core
layout(location=0) in vec4 vertex_pos;
layout(location=1) in vec4 vertex_normal;
layout(location=2) in vec2 vertex_uvs;

layout(location=5) uniform mat4 MVP;
layout(location=6) uniform mat4 M;

out vec4 vertex_pos_out;
out vec4 vertex_normal_out;
out vec2 vertex_uvs_out;

void main()
	{
	gl_Position=MVP*vertex_pos;
	vertex_pos_out=M*vertex_pos;
	vertex_normal_out=M*vertex_normal_out;
	vertex_uvs_out=vertex_uvs;
	}
)EOF"_vert,R"EOF(#version 430 core

layout(location=0) out vec4 color;
in vec4 vertex_pos_out;
in vec4 vertex_normal_out;
in vec2 vertex_uvs_out;

layout(location=0) uniform vec4 view_position;
layout(location=1) uniform vec4 light_position;
layout(location=2) uniform vec3 light_color;
layout(location=3) uniform vec3 ambient;
layout(location=4) uniform vec3 albedo;

void main()
	{
	vec4 VL=light_position - vertex_pos_out;
	vec4 LC=view_position - light_position;
	float vl_size2=dot(VL,VL);
	float lc_size2=dot(LC,LC);
	vec4 vl=VL/sqrt(vl_size2);
	vec3 diffuse=max(dot(vertex_normal_out,vl),0.0)*light_color/vl_size2;
	color=vec4( (ambient + diffuse)*albedo/lc_size2 ,1.0f);
	}
)EOF"_frag);
	}

SceneRenderer::~SceneRenderer() noexcept
	{s_program.release();}

void SceneRenderer::render(const Site& s,const Viewpoint& v
	,CameraBuffer& render_result) noexcept
	{
	render_result.get<0>().filter(Angle::MagFilter::NEAREST)
		.filter(Angle::MinFilter::NEAREST);
	render_result.get<1>().filter(Angle::MagFilter::NEAREST)
		.filter(Angle::MinFilter::NEAREST);
	m_fb.attachColorBuffer<0>(render_result.get<0>())
		.attachDepthBuffer(render_result.get<1>());
	m_fb.bind(Angle::Framebuffer::Target::DRAW);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glViewport(0,0,render_result.get<0>().width()
		,render_result.get<0>().height());

//	TODO: render stuff from s using v

	render_result.get<0>().mipmapsGenerate()
		.filter(Angle::MagFilter::LINEAR)
		.filter(Angle::MinFilter::LINEAR_MIPMAP_LINEAR);
	}

constexpr Angle::VertexAttribute SceneRenderer::ShaderDescriptor::attributes[];

