//@	{
//@	 "targets":[{"name":"textrenderer.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"textrenderer.o","rel":"implementation"}]
//@	}

#ifndef PAGECOMPOSER_TEXTRENDERER_HPP
#define PAGECOMPOSER_TEXTRENDERER_HPP

#include "handle.hpp"
#include <utility>

namespace PageComposer
	{
	class RenderContext;

	class TextRenderer
		{
		public:
			explicit TextRenderer(RenderContext& rc);

			~TextRenderer();

			TextRenderer(TextRenderer&& tr) noexcept:
				 r_rc(tr.r_rc),m_handle(tr.m_handle)
				{tr.m_handle.reset();}

			TextRenderer& operator=(TextRenderer&& tr) noexcept
				{
				std::swap(tr.m_handle,m_handle);
				r_rc=tr.r_rc;
				return *this;
				}

			TextRenderer(const TextRenderer& tr)=delete;
			TextRenderer& operator=(const TextRenderer& tr)=delete;

			const Handle<tr_t>& handle() const noexcept
				{return m_handle;}

			Handle<tr_t>& handle() noexcept
				{return m_handle;}

			RenderContext& renderContext() noexcept
				{return *r_rc;}

		private:
			RenderContext* r_rc;
			Handle<tr_t> m_handle;
		};
	};

#endif
