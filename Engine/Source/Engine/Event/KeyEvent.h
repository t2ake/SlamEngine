#pragma once

#include "Event.h"
#include "Window/KeyCode.h"

#include <sstream>

namespace sl
{

class KeyPressEvent : public Event
{
public:
	KeyPressEvent(int keycode, bool isRepeat = false)
		: m_keyCode(keycode), m_isRepeat(isRepeat) {}

	static EventType GetStaticEventType()
	{
		return EventType::KeyPress;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyPress: " << m_keyCode << (m_isRepeat ? ", repeat" : "");
		return ss.str();
	}

	virtual uint8_t GetCategories() const override
	{
		return SL_EVENT_CATEGORY_INPUT | SL_EVENT_CATEGORY_KEYBOARD;
	}

	int GetKeyCode() const { return m_keyCode; }
	bool IsRepeat() const { return m_isRepeat; }

private:
	int m_keyCode;
	bool m_isRepeat;
};

class KeyReleaseEvent : public Event
{
public:
	KeyReleaseEvent(int keycode) : m_keyCode(keycode) {}

	static EventType GetStaticEventType()
	{
		return EventType::KeyRelease;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyRelease: " << m_keyCode;
		return ss.str();
	}

	virtual uint8_t GetCategories() const override
	{
		return SL_EVENT_CATEGORY_INPUT | SL_EVENT_CATEGORY_KEYBOARD;
	}

	int GetKeyCode() const { return m_keyCode; }

private:
	int m_keyCode;
};

class KeyTypeEvent : public Event
{
public:
	KeyTypeEvent(int keycode) : m_keyCode(keycode) {}

	static EventType GetStaticEventType()
	{
		return EventType::KeyType;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyType: " << m_keyCode;
		return ss.str();
	}
	virtual
		uint8_t GetCategories() const override
	{
		return SL_EVENT_CATEGORY_INPUT | SL_EVENT_CATEGORY_KEYBOARD;
	}

	int GetKeyCode() const { return m_keyCode; }

private:
	int m_keyCode;
};

} // namespace sl
