//@	{
//@	 "targets":[{"name":"pageobjectbase.hpp","type":"include"}]
//@	}

#ifndef PAGECOMPOSER_PAGEOBJECTBASE_HPP
#define PAGECOMPOSER_PAGEOBJECTBASE_HPP

#include "pageobject.hpp"
#include "rendercontext.hpp"
#include "surface.hpp"
#include "layerstack.hpp"

namespace PageComposer
	{
	class PageObjectBase:public PageObject
		{
		public:
			explicit PageObjectBase(RenderContext& rc) noexcept:m_pos_relative(0),r_rc(&rc)
				{
				Vec2 size(r_rc->surface().width(),r_rc->surface().height());
				m_bounding_rect_old.min()=size;
				m_bounding_rect_old.max()=Vec2(0,0);
				}

			PageObjectBase(PageObjectBase&&)=default;

			PageObjectBase& operator=(PageObjectBase&&)=default;

			PageObjectBase& positionAbsolute(Vec2 pos) noexcept final
				{
				dirty_set();
				m_pos=pos;
				m_pos_relative=0;
				return *this;
				}

			PageObjectBase& positionRelative(Vec2 pos) noexcept final
				{
				dirty_set();
				m_pos=pos;
				m_pos_relative=1;
				return *this;
				}

			Vec2 positionAbsolute() const noexcept final
				{
				if(m_pos_relative)
					{
					Vec2 size(r_rc->surface().width(),r_rc->surface().height());
					return 0.5*hadamard(size,m_pos + Vec2{1,1});
					}
				return m_pos;
				}

			Vec2 positionRelative() const noexcept final
				{
				if(m_pos_relative)
					{return m_pos;}
				Vec2 size(r_rc->surface().width(),r_rc->surface().height());
				return hadadiv(2.0*m_pos,size) - Vec2{1,1};
				}

			PageObjectBase& anchor(Vec2 a) noexcept final
				{
				m_anchor=a;
				dirty_set();
				return *this;
				}

			Vec2 anchor() const noexcept final
				{return m_anchor;}

		protected:
			void dirty_set() noexcept
				{
				auto l=layers();
				if(l!=nullptr)
					{(*l)[layerIndex()].dirty(true);}
				}

			void dirty_clear() const noexcept
				{
				auto l=layers();
				if(l!=nullptr)
					{(*l)[layerIndex()].dirty(false);}
				}

			RenderContext& render_context() const noexcept
				{return *r_rc;}

		private:
			Vec2 m_pos;
			Vec2 m_anchor;
			bool m_pos_relative;
			RenderContext* r_rc;
		};
	}

#endif