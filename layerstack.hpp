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
			void render() const noexcept;
			LayerStack& push(Layer&& layer);

		private:
			std::vector<Layer> m_layers;
		};
	}

#endif
