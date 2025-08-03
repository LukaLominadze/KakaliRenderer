#pragma once

#include "Event.h"

class WindowResizedEvent : public Event {
public:
	void Initialize(int width, int height) {
		m_type = EventType::WindowResize;
		m_width = width;
		m_height = height;
		Handled = false;
	}

	inline const int GetWidth() const { return m_width; }
	inline const int GetHeight() const { return m_height; }

	inline static EventType GetStaticType() { return EventType::WindowResize; }
private:
	int m_width, m_height;
};

class WindowClosedEvent : public Event {
public:
	void Initialize() {
		m_type = EventType::WindowClose;
	}

	inline static EventType GetStaticType() { return EventType::WindowClose; }
};
