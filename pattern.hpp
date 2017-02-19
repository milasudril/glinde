//@	{
//@	 "targets":[{"name":"pattern.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"pattern.o","rel":"implementation"}]
//@	}

#ifndef PAGECOMPOSER_PATTERN_HPP
#define PAGECOMPOSER_PATTERN_HPP

#include "pageobjectbase.hpp"

namespace PageComposer
	{
	class Pattern:public PageObjectBase
		{
		public:
			Pattern(const Pattern&)=delete;
			Pattern& operator=(const Pattern&)=delete;

			Pattern(Surface&& surface)=delete;
			Pattern(RenderContext& rc,const Surface& surface);
			~Pattern();
			
			const Handle<pattern_t>& handle() const noexcept
				{return m_handle;}

			Handle<pattern_t>& handle() noexcept
				{return m_handle;}

		private:
			Handle<pattern_t> m_handle;
		};
	}

#endif