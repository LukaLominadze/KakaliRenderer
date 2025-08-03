#pragma once

#include "Event.h"

class MouseButtonEvent : public Event {
public:
	inline int GetButton() const { return m_button; }
protected:
	int m_button;
};

class MouseButtonPressedEvent : public MouseButtonEvent {
public:
	void Initialize(int button) {
		m_type = EventType::MouseButtonPressed;
		m_button = button;
		Handled = false;
	}

	inline static EventType GetStaticType() { return EventType::MouseButtonPressed; }
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
public:
	void Initialize(int button) {
		m_type = EventType::MouseButtonReleased;
		m_button = button;
		Handled = false;
	}

	inline static EventType GetStaticType() { return EventType::MouseButtonReleased; }
};

class MouseMovedEvent : public Event {
public:
	void Initialize(float x, float y) {
		m_type = EventType::MouseMoved;
		m_x = x;
		m_y = y;
		Handled = false;
	}

	inline const float GetX() const { return m_x; }
	inline const float GetY() const { return m_y; }

	inline static EventType GetStaticType() { return EventType::MouseMoved; }
private:
	float m_x, m_y;
};

class MouseScrolledEvent : public Event {
public:
	void Initialize(float x, float y) {
		m_type = EventType::MouseScrolled;
		m_offsetX = x;
		m_offsetY = y;
		Handled = false;
	}

	inline const float GetOffsetX() const { return m_offsetX; }
	inline const float GetOffsetY() const { return m_offsetY; }

	inline static EventType GetStaticType() { return EventType::MouseScrolled; }
private:
	float m_offsetX, m_offsetY;
};
