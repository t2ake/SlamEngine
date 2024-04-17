#pragma once

#include <cstdint>
#include <format>
#include <functional>
#include <string>

namespace sl
{

enum class EventType
{
	None = 0,
	WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowDrop,
	KeyPressed, KeyReleased, KeyTyped,
	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum EventCategory : uint8_t
{
	None = 0,
	EventCategoryWindow   = 1 << 0,
	EventCategoryInput    = 1 << 1,
	EventCategoryKeyboard = 1 << 2,
	EventCategoryMouse    = 1 << 3,
};

class Event
{
protected:
	Event() = default;

public:
	Event(const Event &) = delete;
	Event &operator=(const Event &) = delete;
	Event(Event &&) = delete;
	Event &operator=(Event &&) = delete;
	virtual ~Event() = default;

	virtual EventType GetEventType() const = 0;
	virtual std::string ToString() const = 0;

	virtual uint8_t GetCategories() const = 0;
	bool IsInCategory(EventCategory category) const
	{
		return category & GetCategories();
	}

	void SetHandled(bool handled) { m_handled = handled; }
	bool &GetIsHandled() { return m_handled; }
	bool GetIsHandled() const { return m_handled; }

private:
	bool m_handled = false;
};

class EventDispatcher
{
public:
	EventDispatcher(Event &event) : m_event(event) {}

	template<typename T>
	bool Dispatch(std::function<bool(T &)> func)
	{
		static_assert(std::is_base_of_v<Event, T>);

		// Call func if type of m_event is T.
		if (T::GetStaticEventType() == m_event.GetEventType())
		{
			m_event.GetIsHandled() |= func(static_cast<T &>(m_event));
			return true;
		}

		return false;
	}

private:
	Event &m_event;
};

using EventCallback = std::function<void(Event &)>;

} // namespace sl

template<>
struct std::formatter<sl::Event> : std::formatter<std::string>
{
	auto format(const sl::Event &event, std::format_context &context) const
	{
		return formatter<string>::format(event.ToString(), context);
	}
};
