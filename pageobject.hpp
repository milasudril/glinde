//@	{
//@	 "targets":[{"name":"pageobject.hpp","type":"include"}]
//@	}

#ifndef PAGECOMPOSER_PAGEOBJECT_HPP
#define PAGECOMPOSER_PAGEOBJECT_HPP

#include "rectangle.hpp"

namespace PageComposer
	{
	class Layer;

	class PageObject
		{
		public:
			PageObject():r_layer(nullptr)
				{}

			virtual ~PageObject()=default;

			PageObject& layer(Layer& layer) noexcept
				{
				r_layer=&layer;
				return *this;
				}

			const Layer& layer() const noexcept
				{return *r_layer;}

			Layer& layer() noexcept
				{return *r_layer;}

			virtual PageObject& positionAbsolute(Vec2 pos) noexcept=0;

			virtual PageObject& positionRelative(Vec2 pos) noexcept=0;

			virtual Vec2 positionAbsolute() const noexcept=0;

			virtual Vec2 positionRelative() const noexcept=0;

			virtual PageObject& anchor(Vec2 a) noexcept=0;

			virtual Vec2 anchor() const noexcept=0;

			virtual Rectangle boundingRectangle() const noexcept=0;

			virtual void render() const noexcept=0;

		private:
			Layer* r_layer;
		};
	}

#endif