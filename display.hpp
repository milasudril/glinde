//@	{
//@	 "targets":[{"name":"display.hpp","type":"include"}]
//@	}

#ifndef GLINDE_DISPLAY_HPP
#define GLINDE_DISPLAY_HPP

#include "imagerenderer.hpp" //Must go first since window includes GLFW, which includes GL.
#include "log/logwriter.hpp"
#include "variant.hpp"

#define GLFWMM_NO_MEMBER_ADVERTISMENT

#include "errormessage.hpp"
#include "variant.hpp"
#include "glfwmm/window.hpp"
#include "angle/contextguard.hpp"
#include "angle/init.hpp"
#include "angle/exceptionhandler.hpp"

namespace Glinde
	{
	class Scene;
	class UICallback;

	class Display:public GLFWmm::Window<UICallback>
		{
		public:
			explicit Display(UICallback& cb,GLFWmm::Session& session):
				GLFWmm::Window<UICallback>(0.5,0.5,"Glinde",cb,session)
				,m_context(*this)
				{}

			void draw(const Angle::Texture2D& texture)
				{m_imgrenderer.render(texture);}

		private:
			struct APIInitializer:public Angle::ExceptionHandler
				{
				APIInitializer()
					{
					r_eh_old=&Angle::exceptionHandlerSet(this);

					Glinde::logWrite(Glinde::Log::MessageType::INFORMATION
						,"Initializing GLEW version #0;.#1;"
						,{GLEW_VERSION_MAJOR,GLEW_VERSION_MINOR});

					auto response=Angle::init();
					Glinde::logWrite(Glinde::Log::MessageType::INFORMATION
						,"Got an OpenGL context with the following characteristics:\n"
						"    Vendor:          #0;\n"
						"    Renderer:        #1;\n"
						"    Version:         #2;\n"
						"    Shader language: #3;\n"
						,{response.vendor,response.renderer,response.version,response.glsl_version});
					}
				~APIInitializer()
					{Angle::exceptionHandlerSet(r_eh_old);}

				void raise(const Angle::Error& err) override
					{throw ErrorMessage("An error occured during a call to the graphics backend. #0;",{err.message()});}

				Angle::ExceptionHandler* r_eh_old;
				};

			Angle::ContextGuard<Display> m_context;
			APIInitializer m_initializer;
			ImageRenderer m_imgrenderer;
		};
	}

#endif
