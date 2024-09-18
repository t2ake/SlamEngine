#pragma once

#include <cstdint>
#include <format>
#include <functional>

namespace sl
{

enum class EventType
{
	None = 0,
	WindowClose, WindowResize, WindowGetFocus, WindowLostFocus, WindowDrop,
	KeyPress, KeyRelease, KeyType,
	MouseButtonPress, MouseButtonRelease, MouseButtonAcross, MouseMove, MouseScroll,
	SceneViewportResize, SceneViewportGetFocus, SceneViewportLostFocus, SceneViewportHover,
	CameraActivate, CameraDeactivate,
};

#define SL_EVENT_CATEGORY_NONE           UINT8_C(0x00)
#define SL_EVENT_CATEGORY_WINDOW         UINT8_C(0x01)
#define SL_EVENT_CATEGORY_INPUT          UINT8_C(0x02)
#define SL_EVENT_CATEGORY_KEYBOARD       UINT8_C(0x04)
#define SL_EVENT_CATEGORY_MOUSE          UINT8_C(0x08)
#define SL_EVENT_CATEGORY_SCENE_VIEWPORT UINT8_C(0x10)
#define SL_EVENT_CATEGORY_CAMERA         UINT8_C(0x11)

#define BIND_EVENT_CALLBACK(fun) std::bind_front(&fun, this)

class Event
{
public:
	virtual ~Event() = default;

	virtual EventType GetEventType() const = 0;
	virtual std::string ToString() const = 0;

	virtual uint8_t GetCategories() const = 0;
	bool IsInCategory(uint8_t category) const
	{
		return category & GetCategories();
	}

	void SetHandled(bool handled) { m_handled = handled; }
	bool &IsHandled() { return m_handled; }
	bool IsHandled() const { return m_handled; }

private:
	bool m_handled = false;
};

class EventDispatcher
{
public:
	EventDispatcher(Event &event) : m_event(event) {}

	template<class DerivedEvent>
	bool Dispatch(auto fun)
	{
		static_assert(std::is_base_of_v<Event, DerivedEvent>);
		static_assert(requires{ DerivedEvent::GetStaticEventType(); });

		// Call fun if type of m_event is DerivedEvent.
		if (m_event.GetEventType() == DerivedEvent::GetStaticEventType())
		{
			m_event.IsHandled() |= fun(static_cast<DerivedEvent &>(m_event));
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
