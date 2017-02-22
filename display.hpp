//@	{
//@	 "targets":[{"name":"display.hpp","type":"include"}]
//@	}

#ifndef GLINDE_DISPLAY_HPP
#define GLINDE_DISPLAY_HPP

#include "imagerenderer.hpp" //Must go first since window includes GLFW, which includes GL.

#define GLFWMM_NO_MEMBER_ADVERTISMENT

#include "glfwmm/window.hpp"
//#include "uicallback.hpp"
#include "angle/contextguard.hpp"
#include "angle/init.hpp"

namespace Glinde
	{
	class Scene;
	class UICallback;

	class Display:public GLFWmm::Window<UICallback>
		{
		public:
			Display(UICallback& cb,GLFWmm::Session& session):
				GLFWmm::Window<UICallback>(0.5,0.5,"Glinde",cb,session)
				,m_context(*this)
				{}

			void draw(const Angle::Texture2D& texture)
				{m_imgrenderer.render(texture);}

		private:
			struct APIInitializer
				{
				APIInitializer()
					{
					auto response=Angle::init();
					fprintf(stderr,"Got an OpenGL context with the following characteristics:\n"
						"    Vendor:          %s\n"
						"    Renderer:        %s\n"
						"    Version:         %s\n"
						"    Shader language: %s\n"
						,response.vendor,response.renderer,response.version,response.glsl_version);
					}
				};

			Angle::ContextGuard<Display> m_context;
			APIInitializer m_initializer;
			ImageRenderer m_imgrenderer;
		};
	}

#endif
