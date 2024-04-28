#include "LayerStack.h"

namespace sl
{

LayerStack::~LayerStack()
{
	// PENDING: Should I iterate them from the top to the bottom?
	for (Layer *pLayer : m_pLayers)
	{
		delete pLayer;
	}
	m_pLayers.clear();
}

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

} // namespace sl
