#pragma once

#include "Layer/Layer.h"

#include <vector>

namespace sl
{

class LayerStack final
{
public:
	LayerStack() = default;
	LayerStack(const LayerStack &) = delete;
	LayerStack &operator=(const LayerStack &) = delete;
	LayerStack(LayerStack &&) = delete;
	LayerStack &operator=(LayerStack &&) = delete;
	~LayerStack();

	void PushLayer(Layer *pLayer);
	void PopLayer(Layer *pLayer);

	auto begin() { return m_pLayers.begin(); }
	auto end() { return m_pLayers.end(); }

private:
	std::vector<Layer *> m_pLayers;
};

} // namespace sl
