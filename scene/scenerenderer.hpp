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
//@	 "targets":[{"name":"scenerenderer.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"scenerenderer.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_SCENERENDERER_HPP
#define GLINDE_SCENERENDERER_HPP

#include "../angle/vertexarray.hpp"
#include "../angle/framebuffer.hpp"
#include "../angle/texture2d.hpp"

namespace Glinde
	{
	class Viewpoint;
	class Site;

	class SceneRenderer
		{
		public:
			SceneRenderer();
			~SceneRenderer() noexcept;

			void framebufferResize(int width,int height);

			void render(const Site& site,const Viewpoint& v) noexcept;

			const Angle::Texture2D& texture() const noexcept
				{return m_texture_out;}

		private:
			struct ShaderDescriptor
				{
				static constexpr Angle::VertexAttribute attributes[]=
					{
					 {4,Angle::ConstantGet<float>::value}
					,{4,Angle::ConstantGet<float>::value}
					,{2,Angle::ConstantGet<float>::value}
					};
				};

			Angle::VertexArray<ShaderDescriptor> m_vao;
			Angle::Framebuffer m_fb;
			Angle::Texture2D m_texture_out;
			Angle::Texture2D m_depthbuffer;
		};
	};

#endif
