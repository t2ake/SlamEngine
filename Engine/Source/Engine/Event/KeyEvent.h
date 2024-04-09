#pragma once

#include "Event.h"

#include <sstream>

namespace sl
{

class KeyPressedEvent : public Event
{
public:
	KeyPressedEvent(uint32_t keycode, bool isRepeat = false)
		: m_keyCode(keycode), m_isRepeat(isRepeat) {}

	static EventType GetStaticEventType()
	{
		return EventType::KeyPressed;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << m_keyCode << (m_isRepeat ? ", repeat" : "");
		return ss.str();
	}

	virtual uint8_t GetCategories() const override
	{
		return EventCategoryInput | EventCategoryKeyboard;
	}

	uint32_t GetKeyCode() const { return m_keyCode; }
	bool IsRepeat() const { return m_isRepeat; }

private:
	uint32_t m_keyCode;
	bool m_isRepeat;
};

class KeyReleasedEvent : public Event
{
public:
	KeyReleasedEvent(const uint32_t keycode) : m_keyCode(keycode) {}

	static EventType GetStaticEventType()
	{
		return EventType::KeyReleased;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << m_keyCode;
		return ss.str();
	}

	virtual uint8_t GetCategories() const override
	{
		return EventCategoryInput | EventCategoryKeyboard;
	}

	uint32_t GetKeyCode() const { return m_keyCode; }

private:
	uint32_t m_keyCode;
};

class KeyTypedEvent : public Event
{
public:
	KeyTypedEvent(const uint32_t keycode) : m_keyCode(keycode) {}


	static EventType GetStaticEventType()
	{
		return EventType::KeyTyped;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyTypedEvent: " << m_keyCode;
		return ss.str();
	}
	virtual
		uint8_t GetCategories() const override
	{
		return EventCategoryInput | EventCategoryKeyboard;
	}

	uint32_t GetKeyCode() const { return m_keyCode; }

private:
	uint32_t m_keyCode;
};

} // namespace sl
