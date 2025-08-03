#pragma once

#include "Event.h"

class KeyEvent : public Event {
public:
	const int GetKeyCode() const { return m_keyCode; }
protected:
	int m_keyCode;
};

class KeyPressedEvent : public KeyEvent {
public:
	void Initialize(int keyCode) {
		m_type = EventType::KeyPressed;
		m_keyCode = keyCode;
		Handled = false;
	}

	inline static EventType GetStaticType() { return EventType::KeyPressed; }
};

class KeyReleasedEvent : public KeyEvent {
public:
	void Initialize(int keyCode) {
		m_type = EventType::KeyPressed;
		m_keyCode = keyCode;
		Handled = false;
	}

	inline static EventType GetStaticType() { return EventType::KeyReleased; }
};

class KeyTypedEvent : public KeyEvent {
public:
	void Initialize(int keyCode) {
		m_type = EventType::KeyTyped;
		m_keyCode = keyCode;
		Handled = false;
	}

	inline static EventType GetStaticType() { return EventType::KeyTyped; }
};

