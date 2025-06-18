#pragma once

#include "gl/OrthographicCamera.h"
#include "events/Event.h"
#include "events/MouseEvent.h"
#include "events/ApplicationEvent.h"
#include "layers/Layer.h"

class OrthographicCameraController : public Layer {
public:
	OrthographicCameraController();
	~OrthographicCameraController();

	void SetProjection(float aspectRatio, float near, float far, bool rotation = true);

	inline const OrthographicCamera& GetCamera() const { return m_camera; }

	void OnEvent(Event& event) override;
	void OnUpdate(float timestep) override;
	void OnRender() override;
private:
	bool OnMouseScrolled(MouseScrolledEvent& e);
	bool OnWindowResized(WindowResizedEvent& e);
	bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
	bool OnEscPressed(KeyPressedEvent& e);
private:
	float m_aspectRatio;
	float m_zoom = 1.0f;
	float m_near, m_far;
	OrthographicCamera m_camera;

	bool m_rotation = false;
	bool m_mouseLocked = false;

	float m_cameraTranslationSpeed = 3.0f, m_cameraRotationSpeed = 180.0f;
};

