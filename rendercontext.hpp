//@	{
//@	 "targets":[{"name":"rendercontext.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"rendercontext.o","rel":"implementation"}]
//@	}

#ifndef PAGECOMPOSER_RENDERCONTEXT_HPP
#define PAGECOMPOSER_RENDERCONTEXT_HPP

#include "handle.hpp"
#include <utility>

namespace PageComposer
	{
	class Surface;
	class RenderContext
		{
		public:
			explicit RenderContext(Surface& surf);

			~RenderContext();

			RenderContext(RenderContext&& rc) noexcept:
				 m_handle(rc.m_handle),r_surface(rc.r_surface)
				{rc.m_handle.reset();}

			RenderContext& operator=(RenderContext&& rc) noexcept
				{
				std::swap(rc.m_handle,m_handle);
				std::swap(rc.r_surface,r_surface);
				return *this;
				}

			RenderContext(const RenderContext& rc)=delete;
			RenderContext& operator=(const RenderContext& rc)=delete;


			const Handle<rc_t>& handle() const noexcept
				{return m_handle;}

			Handle<rc_t>& handle() noexcept
				{return m_handle;}

			const Surface& surface() const noexcept
				{return *r_surface;}

			Surface& surface() noexcept
				{return *r_surface;}
	
		private:
			Handle<rc_t> m_handle;
			Surface* r_surface;
		};
	};

#endif
