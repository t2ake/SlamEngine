#pragma once

#include "Core/Log.h"

#include <concepts>
#include <cstdint>
#include <functional>

namespace sl
{

enum class EventType
{
	None = 0,
	WindowClose, WindowResize, WindowMinimize, WindowMaximize, WindowRestore, WindowGetFocus, WindowLostFocus, WindowDrop,
	KeyPress, KeyRelease, KeyType,
	MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll,
	SceneViewportResize, SceneViewportGetFocus, SceneViewportLostFocus, SceneViewportHover,
};

#define SL_EVENT_CATEGORY_NONE           UINT8_C(0x00)
#define SL_EVENT_CATEGORY_WINDOW         UINT8_C(0x01)
#define SL_EVENT_CATEGORY_INPUT          UINT8_C(0x02)
#define SL_EVENT_CATEGORY_KEYBOARD       UINT8_C(0x04)
#define SL_EVENT_CATEGORY_MOUSE          UINT8_C(0x08)
#define SL_EVENT_CATEGORY_SCENE_VIEWPORT UINT8_C(0x10)

#define SL_EVENT_CATEGORY_ALL ( \
		SL_EVENT_CATEGORY_WINDOW | \
		SL_EVENT_CATEGORY_INPUT | \
		SL_EVENT_CATEGORY_KEYBOARD | \
		SL_EVENT_CATEGORY_MOUSE | \
		SL_EVENT_CATEGORY_SCENE_VIEWPORT)

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

template<class T, class Fun>
concept DispatchableDerivedEvent = requires(T e, Fun fun)
{
	requires std::is_base_of_v<Event, T>;
	{ T::GetStaticEventType() } -> std::same_as<EventType>;
	{ fun(e) } -> std::same_as<bool>;
};

class EventDispatcher
{
public:
	EventDispatcher(Event &event) : m_event(event) {}

	template<class T, class Fun> requires DispatchableDerivedEvent<T, Fun>
	bool Dispatch(Fun fun)
	{
		// Call fun if type of m_event is T.
		if (m_event.GetEventType() == T::GetStaticEventType())
		{
			m_event.IsHandled() |= fun(static_cast<T &>(m_event));
			return true;
		}

		return false;
	}

private:
	Event &m_event;
};

using EventCallback = std::function<void(Event &)>;

} // namespace sl

#if !defined(SL_FINAL)

template<>
struct fmt::formatter<sl::Event> : fmt::formatter<std::string>
{
	auto format(const sl::Event &event, format_context &ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "{}", event.ToString());
	}
};

#endif
