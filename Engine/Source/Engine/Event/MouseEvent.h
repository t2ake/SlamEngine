#pragma once

#include "Event.h"
#include "Window/MouseButtonCode.h"

#include <glm/vec2.hpp>

#include <sstream>

namespace sl
{

class MouseButtonPressEvent : public Event
{
public:
	MouseButtonPressEvent(int button) : m_button(button) {}

	static EventType GetStaticEventType()
	{
		return EventType::MouseButtonPress;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonPress: " << m_button;
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

class MouseButtonReleaseEvent : public Event
{
public:
	MouseButtonReleaseEvent(const int button) : m_button(button) {}

	static EventType GetStaticEventType()
	{
		return EventType::MouseButtonRelease;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonRelease: " << m_button;
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

class MouseButtonAcrossEvent : public Event
{
public:
	MouseButtonAcrossEvent() = default;

	static EventType GetStaticEventType()
	{
		return EventType::MouseButtonAcross;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		return "MouseButtonAcross";
	}

	virtual uint8_t GetCategories() const override
	{
		return SL_EVENT_CATEGORY_INPUT | SL_EVENT_CATEGORY_MOUSE;
	}
};

class MouseMoveEvent : public Event
{
public:
	MouseMoveEvent(float x, float y) : m_posX(x), m_posY(y) {}

	static EventType GetStaticEventType()
	{
		return EventType::MouseMove;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseMove: " << m_posX << ", " << m_posY;
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

class MouseScrollEvent : public Event
{
public:
	MouseScrollEvent(float xOffset, float yOffset) : m_offsetX(xOffset), m_offsetY(yOffset) {}

	static EventType GetStaticEventType()
	{
		return EventType::MouseScroll;
	}

	virtual EventType GetEventType() const override
	{
		return GetStaticEventType();
	}

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseScroll: " << m_offsetX << ", " << m_offsetY;
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
