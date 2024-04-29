#pragma once

#include "Layer/Layer.h"

#include <vector>

namespace sl
{

class LayerStack final
{
public:
	LayerStack() = default;
	~LayerStack();

	void PushLayer(Layer *pLayer);
	void PopLayer(Layer *pLayer);

	std::vector<Layer *> &GetLayers() { return m_pLayers; }
	const std::vector<Layer *> &GetLayers() const { return m_pLayers; }

	auto begin() { return m_pLayers.begin(); }
	auto end() { return m_pLayers.end(); }
	auto begin() const { return m_pLayers.cbegin(); }
	auto end() const { return m_pLayers.cend(); }

	auto rbegin() { return std::make_reverse_iterator(m_pLayers.begin()); }
	auto rend() { return std::make_reverse_iterator(m_pLayers.end()); }
	auto crbegin() const { return std::make_reverse_iterator(m_pLayers.cbegin()); }
	auto crend() const { return std::make_reverse_iterator(m_pLayers.cend()); }

private:
	std::vector<Layer *> m_pLayers;
};

} // namespace sl
