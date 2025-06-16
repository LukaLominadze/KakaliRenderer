#pragma once

#include "events/Event.h"
#include "events/ApplicationEvent.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

class Layer {
public:
	Layer() = default;
	virtual ~Layer() = default;

	virtual void OnAttach() {}
	virtual void OnEvent(Event& evnet) {}
	virtual void OnUpdate(float timestep) {}
	virtual void OnRender() {}
	virtual void OnImGuiRender() {}
	virtual void OnDetach() {}
};