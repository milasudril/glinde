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
			Layer(int z_index,PageObject& object) noexcept:m_z_index(z_index)
				,m_dirty(0),r_object(&object)
				{object.layer(*this);}

			bool operator<(const Layer& layer) const noexcept
				{return m_z_index<layer.m_z_index;}

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
			int m_z_index;
			bool m_dirty;
			PageObject* r_object;
		};
	}

#endif
