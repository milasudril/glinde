//@	{"targets":[{"name":"framebuffer.hpp","type":"include"}]}

#ifndef ANGLE_FRAMEBUFFER_HPP
#define ANGLE_FRAMEBUFFER_HPP

#include "texture2d.hpp"
#include <utility>
#include <array>

namespace Angle
	{
	class Framebuffer
		{
		public:
			enum class Target:GLenum{DRAW=GL_DRAW_FRAMEBUFFER,READ=GL_READ_FRAMEBUFFER,READDRAW=GL_FRAMEBUFFER};

			Framebuffer()
				{glCreateFramebuffers(1,&m_handle);}

			static Framebuffer framebufferDefault() noexcept
				{return Framebuffer(0);}

			~Framebuffer()
				{
				if(m_handle!=0)
					{
					glBindFramebuffer(GL_FRAMEBUFFER,0);
					glDeleteFramebuffers(1,&m_handle);
					}
				}

			Framebuffer(const Framebuffer&)=delete;
			Framebuffer& operator=(const Framebuffer&)=delete;
			
			Framebuffer(Framebuffer&& obj) noexcept:m_handle(obj.m_handle)
				{obj.m_handle=0;}

			Framebuffer& operator=(Framebuffer&& obj)
				{
				std::swap(obj.m_handle,m_handle);
				return *this;
				}

			template<int N>
			Framebuffer& attachColorBuffer(Texture2D& texture,GLint level=0) noexcept
				{
			//	API defect?: Limited compile-time range, but the spec sais unlimited...
				static_assert(N>=0,"");
				static_assert((N<32 && GL_COLOR_ATTACHMENT0 + 32==GL_DEPTH_ATTACHMENT)
					|| (GL_COLOR_ATTACHMENT0 + 32!=GL_DEPTH_ATTACHMENT),"");

				glNamedFramebufferTexture(m_handle,N+GL_COLOR_ATTACHMENT0,texture.handle(),level);
				return *this;
				}

			Framebuffer& attachDepthBuffer(Texture2D& texture,GLint level) noexcept
				{
				glNamedFramebufferTexture(m_handle,GL_DEPTH_ATTACHMENT,texture.handle(),level);
				return *this;
				}

			Framebuffer& attachStencilBuffer(Texture2D& texture,GLint level) noexcept
				{
				glNamedFramebufferTexture(m_handle,GL_STENCIL_ATTACHMENT,texture.handle(),level);
				return *this;
				}

			template<class ... T>
			void colorBuffersOutputActivate(T ... vals) noexcept
				{
				std::array<unsigned int,sizeof...(vals)> val_array{vals...};
				auto ptr=val_array.begin();
				while(ptr!=val_array.end())
					{
					*ptr+=GL_COLOR_ATTACHMENT0;
					++ptr;
					}
				glNamedFramebufferDrawBuffers(m_handle,val_array.size(),val_array.begin());
				}

			void bind(Target t) noexcept
				{glBindFramebuffer(native_type(t),m_handle);}

		private:
			Framebuffer(GLuint id):m_handle(id){}

			static constexpr GLenum native_type(Target t) noexcept
				{return static_cast<GLenum>(t);}
			GLuint m_handle;
		};
	}

#endif