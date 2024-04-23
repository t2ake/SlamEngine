#pragma once

#include "Event/Event.h"

namespace sl
{

class Layer
{
protected:
	Layer() = default;

public:
	Layer(const Layer &) = delete;
	Layer &operator=(const Layer &) = delete;
	Layer(Layer &&) = delete;
	Layer &operator=(Layer &&) = delete;
	virtual ~Layer() = default;

	virtual void OnAttach() = 0;
	virtual void OnDetach() = 0;
	virtual void OnEvent(Event &event) = 0;
	
	virtual void BeginFrame() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;
	virtual void EndFrame() = 0;

	void SetName(std::string name) { m_name = std::move(name); }
	std::string &GetName() { return m_name; }
	const std::string &GetName() const { return m_name; }

private:
	std::string m_name = "Default Layer Name";
};

} // namespace sl
