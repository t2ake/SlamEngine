#include "LayerStack.h"

namespace sl
{

void LayerStack::PushLayer(Layer *pLayer)
{
	pLayer->OnAttach();
	m_pLayers.emplace_back(pLayer);
}

void LayerStack::PopLayer(Layer *pLayer)
{
	pLayer->OnDetach();
	auto it = std::find(m_pLayers.begin(), m_pLayers.end(), pLayer);
	if (it != m_pLayers.end())
	{
		m_pLayers.erase(it);
	}
}

void LayerStack::Shutdown()
{
	for (Layer *pLayer : m_pLayers)
	{
		delete pLayer;
	}
	m_pLayers.clear();
}

} // namespace sl
