#pragma once

#include "Event.h"

#include <sstream>

namespace sl
{

class MouseButtonPressedEvent : public Event
{
public:
	using Event::Event;

	MouseButtonPressedEvent(uint32_t button) : m_button(button) {}

	static EventType GetStaticEventType()
	{
		return EventType::MouseButtonPressed;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << m_button;
		return ss.str();
	}

	virtual uint8_t GetCategories() const override
	{
		return EventCategoryInput | EventCategoryMouse;
	}

	uint32_t GetButton() const { return m_button; }

private:
	uint32_t m_button;
};

class MouseButtonReleasedEvent : public Event
{
public:
	using Event::Event;

	MouseButtonReleasedEvent(const uint32_t button) : m_button(button) {}

	static EventType GetStaticEventType()
	{
		return EventType::MouseButtonReleased;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << m_button;
		return ss.str();
	}

	virtual uint8_t GetCategories() const override
	{
		return EventCategoryInput | EventCategoryMouse;
	}

	uint32_t GetButton() const { return m_button; }

private:
	uint32_t m_button;
};

class MouseMovedEvent : public Event
{
public:
	using Event::Event;

	MouseMovedEvent(float x, float y) : m_posX(x), m_posY(y) {}

	static EventType GetStaticEventType()
	{
		return EventType::MouseMoved;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseMovedEvent: " << m_posX << ", " << m_posY;
		return ss.str();
	}

	virtual uint8_t GetCategories() const override
	{
		return EventCategoryInput | EventCategoryMouse;
	}

	float GetPosX() const { return m_posX; }
	float GetPosY() const { return m_posY; }

private:
	float m_posX, m_posY;
};

class MouseScrolledEvent : public Event
{
public:
	using Event::Event;

	MouseScrolledEvent(float xOffset, float yOffset) : m_offsetX(xOffset), m_offsetY(yOffset) {}

	static EventType GetStaticEventType()
	{
		return EventType::MouseScrolled;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseScrolledEvent: " << m_offsetX << ", " << m_offsetY;
		return ss.str();
	}

	virtual uint8_t GetCategories() const override
	{
		return EventCategoryInput | EventCategoryMouse;
	}

	float GetOffsetX() const { return m_offsetX; }
	float GetOffsetY() const { return m_offsetY; }

private:
	float m_offsetX, m_offsetY;
};

} // namespace sl
