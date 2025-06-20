#pragma once

#include "gl/PerspectiveCamera.h"
#include "events/Event.h"
#include "events/MouseEvent.h"
#include "events/ApplicationEvent.h"
#include "events/KeyEvent.h"
#include "layers/Layer.h"


class PerspectiveCameraController : public Layer {
public:
	PerspectiveCameraController();
	~PerspectiveCameraController();

	void SetProjection(float aspectRatio, float fov = 60.0f);

	inline PerspectiveCamera& GetCamera() { return m_camera; }

	void OnEvent(Event& event) override;
	void OnUpdate(float timestep) override;
private:
	bool OnMouseScrolled(MouseScrolledEvent& e);
	bool OnWindowResized(WindowResizedEvent& e);
	bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
	bool OnEscPressed(KeyPressedEvent& e);
private:
	float m_aspectRatio = 1.0f;
	float m_fov = 60.0f;
	float m_zoom = 1.0f;
	float m_mouseMovedX = 0.0f, m_mouseMovedY = 0.0f;
	PerspectiveCamera m_camera;

	float m_cameraTranslationSpeed = 3.0f, m_cameraRotationSpeed = 90.0f;

	bool m_mouseLocked = false;
};

