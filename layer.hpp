//@	{
//@	 "targets":[{"name":"layer.hpp","type":"include"}]
//@	}

#ifndef PAGECOMPOSER_LAYER_HPP
#define PAGECOMPOSER_LAYER_HPP
#include "pageobject.hpp"

namespace PageComposer
	{
	class Layer
		{
		public:
			explicit Layer(PageObject& object) noexcept:
				m_dirty(1),r_object(&object)
				{}

			Layer& dirty(bool d) noexcept
				{
				m_dirty=d;
				return *this;
				}

			bool dirty() const noexcept
				{return m_dirty;}

			PageObject& object() noexcept
				{return *r_object;}

			const PageObject& object() const noexcept
				{return *r_object;}

		private:
			bool m_dirty;
			PageObject* r_object;
		};
	}

#endif
