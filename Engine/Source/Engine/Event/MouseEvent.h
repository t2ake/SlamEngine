#pragma once

#include "Event.h"

#include <glm/vec2.hpp>

#include <sstream>

namespace sl
{

class MouseButtonPressedEvent : public Event
{
public:
	using Event::Event;

	MouseButtonPressedEvent(int button) : m_button(button) {}

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
		return SL_EVENT_CATEGORY_INPUT | SL_EVENT_CATEGORY_MOUSE;
	}

	int GetButton() const { return m_button; }

private:
	int m_button;
};

class MouseButtonReleasedEvent : public Event
{
public:
	using Event::Event;

	MouseButtonReleasedEvent(const int button) : m_button(button) {}

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
		return SL_EVENT_CATEGORY_INPUT | SL_EVENT_CATEGORY_MOUSE;
	}

	int GetButton() const { return m_button; }

private:
	int m_button;
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
		return SL_EVENT_CATEGORY_INPUT | SL_EVENT_CATEGORY_MOUSE;
	}

	glm::vec2 GetPosition() const { return { m_posX , m_posY }; }
	float GetPositionX() const { return m_posX; }
	float GetPositionY() const { return m_posY; }

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
		return SL_EVENT_CATEGORY_INPUT | SL_EVENT_CATEGORY_MOUSE;
	}

	float GetOffsetX() const { return m_offsetX; }
	float GetOffsetY() const { return m_offsetY; }

private:
	float m_offsetX, m_offsetY;
};

} // namespace sl
