#pragma once

#include "Event.h"
#include "Window/MouseButtonCode.h"

#include <glm/vec2.hpp>

#include <format>

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
		return std::format("MouseButtonPress: {}", m_button);
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
		return std::format("MouseButtonRelease: {}", m_button);
	}

	virtual uint8_t GetCategories() const override
	{
		return SL_EVENT_CATEGORY_INPUT | SL_EVENT_CATEGORY_MOUSE;
	}

	int GetButton() const { return m_button; }

private:
	int m_button;
};

class MouseMoveEvent : public Event
{
public:
	MouseMoveEvent(int x, int y) : m_posX(x), m_posY(y) {}

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
		return std::format("MouseMove: {}, {}", m_posX, m_posY);
	}

	virtual uint8_t GetCategories() const override
	{
		return SL_EVENT_CATEGORY_INPUT | SL_EVENT_CATEGORY_MOUSE;
	}

	glm::ivec2 GetPosition() const { return { m_posX , m_posY }; }
	int GetPositionX() const { return m_posX; }
	int GetPositionY() const { return m_posY; }

private:
	int m_posX, m_posY;
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
		return std::format("MouseScroll: {}, {}", m_offsetX, m_offsetY);
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
