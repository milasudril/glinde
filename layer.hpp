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
			Layer(PageObject& object) noexcept:
				m_dirty(0),r_object(&object)
				{object.layer(*this);}

			void render() const noexcept
				{
				if(m_dirty)
					{r_object->render();}
				}

			Layer& dirty(bool d) noexcept
				{
				m_dirty=d;
				return *this;
				}

			bool dirty() const noexcept
				{return m_dirty;}

		private:
			bool m_dirty;
			PageObject* r_object;
		};
	}

#endif
