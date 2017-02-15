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
			RenderContext(Surface& surf);

			~RenderContext();

			RenderContext(RenderContext&& rc) noexcept:
				 m_handle(rc.m_handle)
				{rc.m_handle.reset();}

			RenderContext& operator=(RenderContext&& rc) noexcept
				{
				std::swap(rc.m_handle,m_handle);
				return *this;
				}

			RenderContext(const RenderContext& rc)=delete;
			RenderContext& operator=(const RenderContext& rc)=delete;


			const Handle<rc_t>& handle() const noexcept
				{return m_handle;}

			Handle<rc_t>& handle() noexcept
				{return m_handle;}
	
		private:
			Handle<rc_t> m_handle;
		};
	};

#endif
