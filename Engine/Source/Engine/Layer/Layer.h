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

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate() {}
	virtual void OnEvent(Event &event) {}

	void SetName(std::string name) { m_name = std::move(name); }
	std::string &GetName() { return m_name; }
	const std::string &GetName() const { return m_name; }

private:
	std::string m_name = "Layer";
};

} // namespace sl
