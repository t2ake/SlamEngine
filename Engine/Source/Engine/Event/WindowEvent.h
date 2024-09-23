#pragma once

#include "Event.h"

#include <sstream>

namespace sl
{

class WindowCloseEvent : public Event
{
public:
	static EventType GetStaticEventType()
	{
		return EventType::WindowClose;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		return "WindowClose";
	}

	virtual uint8_t GetCategories() const override
	{
		return SL_EVENT_CATEGORY_WINDOW;
	}
};

class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(uint32_t width, uint32_t height) : m_width(width), m_height(height) {}

	static EventType GetStaticEventType()
	{
		return EventType::WindowResize;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "WindowResize: " << m_width << ", " << m_height;
		return ss.str();
	}

	virtual uint8_t GetCategories() const override
	{
		return SL_EVENT_CATEGORY_WINDOW;
	}

	uint32_t GetWidth() const { return m_width; }
	uint32_t GetHeight() const { return m_height; }

private:
	uint32_t m_width, m_height;
};

class WindowMinimizeEvent : public Event
{
public:
	static EventType GetStaticEventType()
	{
		return EventType::WindowMinimize;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		return "WindowMinimize";
	}

	virtual uint8_t GetCategories() const override
	{
		return SL_EVENT_CATEGORY_WINDOW;
	}
};

class WindowMaximizeEvent : public Event
{
public:
	static EventType GetStaticEventType()
	{
		return EventType::WindowMaximize;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		return "WindowMaximize";
	}

	virtual uint8_t GetCategories() const override
	{
		return SL_EVENT_CATEGORY_WINDOW;
	}
};

class WindowRestoreEvent : public Event
{
public:
	static EventType GetStaticEventType()
	{
		return EventType::WindowRestore;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		return "WindowRestore";
	}

	virtual uint8_t GetCategories() const override
	{
		return SL_EVENT_CATEGORY_WINDOW;
	}
};

class WindowGetFocusEvent : public Event
{
public:
	static EventType GetStaticEventType()
	{
		return EventType::WindowGetFocus;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		return "WindowGetFocus";
	}

	virtual uint8_t GetCategories() const override
	{
		return SL_EVENT_CATEGORY_WINDOW;
	}
};

class WindowLossFocusEvent : public Event
{
public:
	static EventType GetStaticEventType()
	{
		return EventType::WindowLostFocus;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		return "WindowLossFocus";
	}

	virtual uint8_t GetCategories() const override
	{
		return SL_EVENT_CATEGORY_WINDOW;
	}
};

// TODO: Support multi drop.
class WindowDropEvent : public Event
{
public:
	WindowDropEvent(std::string path) : m_path(std::move(path)) {}

	static EventType GetStaticEventType()
	{
		return EventType::WindowDrop;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "WindowDrop: " << m_path;
		return ss.str();
	}

	virtual uint8_t GetCategories() const override
	{
		return SL_EVENT_CATEGORY_WINDOW;
	}

	const std::string &GetPath() const { return m_path; }

private:
	std::string m_path;
};

} // namespace sl
