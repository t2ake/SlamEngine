#include "LayerStack.h"

namespace sl
{

LayerStack::~LayerStack()
{
	for (Layer *pLayer : m_pLayers)
	{
		delete pLayer;
	}
}

void LayerStack::PushLayer(Layer *pLayer)
{
	m_pLayers.emplace_back(pLayer);
}

void LayerStack::PopLayer(Layer *pLayer)
{
	auto it = std::find(m_pLayers.begin(), m_pLayers.end(), pLayer);
	if (it != m_pLayers.end())
	{
		m_pLayers.erase(it);
	}
}

} // namespace sl
