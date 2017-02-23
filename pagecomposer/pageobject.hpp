//@	{
//@	 "targets":[{"name":"pageobject.hpp","type":"include"}]
//@	}

#ifndef PAGECOMPOSER_PAGEOBJECT_HPP
#define PAGECOMPOSER_PAGEOBJECT_HPP

#include "rectangle.hpp"
#include <cstddef>

namespace PageComposer
	{
	class LayerStack;

	class PageObject
		{
		public:
			PageObject():r_stack(nullptr),m_index(0)
				{}

			virtual ~PageObject()=default;

			PageObject& layerStack(LayerStack& stack,size_t index) noexcept
				{
				r_stack=&stack;
				m_index=index;
				return *this;
				}

			virtual PageObject& positionAbsolute(Vec2 pos) noexcept=0;

			virtual PageObject& positionRelative(Vec2 pos) noexcept=0;

			virtual Vec2 positionAbsolute() const noexcept=0;

			virtual Vec2 positionRelative() const noexcept=0;

			virtual PageObject& anchor(Vec2 a) noexcept=0;

			virtual Vec2 anchor() const noexcept=0;

			virtual Rectangle boundingRectangle() const noexcept=0;

			virtual void render() const noexcept=0;

			Rectangle boundingRectOld() const noexcept
				{return m_bounding_rect_old;}

		protected:
			LayerStack* layers() const noexcept
				{return r_stack;}

			size_t layerIndex() const noexcept
				{return m_index;}

			mutable Rectangle m_bounding_rect_old;

		private:
			LayerStack* r_stack;
			size_t m_index;
		};
	}

#endif