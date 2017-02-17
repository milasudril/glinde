//@	{
//@	 "targets":[{"name":"layerstack.hpp","type":"include"}]
//@	}

#ifndef PAGECOMPOSER_LAYERSTACK_HPP
#define PAGECOMPOSER_LAYERSTACK_HPP

#include <vector>

namespace PageComposer
	{
	class LayerStack
		{
		public:
			void render() const noexcept
				{
				std::for_each(m_layers.begin(),m_layers.end()
					,[](const Layer& l)
						{l.render();}
					);
				}

			LayerStack& push(Layer&& layer)
				{
				m_layers.push_back(std::move(layer));
				return *this;
				}

			size_t size() const noexcept
				{return m_layers.size();}

			void layersSwap(size_t a,size_t b) noexcept
				{std::swap(m_layers[a],m_layers[b]);}

		private:
			std::vector<Layer> m_layers;
		};
	}

#endif
