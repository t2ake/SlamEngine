#pragma once

#include <memory>
#include <vector>

namespace sl
{

class Layer;

class LayerStack final
{
public:
	LayerStack() = default;
	~LayerStack() = default;

	void PushLayer(std::unique_ptr<Layer> pLayer);
	// TODO: PopLayer

	void BeginFrame();
	void Update(float deltaTime);
	void Render();
	void EndFrame();

	auto begin() { return m_pLayers.begin(); }
	auto end() { return m_pLayers.end(); }
	auto begin() const { return m_pLayers.cbegin(); }
	auto end() const { return m_pLayers.cend(); }

	auto rbegin() { return std::make_reverse_iterator(m_pLayers.begin()); }
	auto rend() { return std::make_reverse_iterator(m_pLayers.end()); }
	auto crbegin() const { return std::make_reverse_iterator(m_pLayers.cbegin()); }
	auto crend() const { return std::make_reverse_iterator(m_pLayers.cend()); }

private:
	std::vector<std::unique_ptr<Layer>> m_pLayers;
};

} // namespace sl
