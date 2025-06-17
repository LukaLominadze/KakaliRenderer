#include "pch.h"

#include "OrthographicCameraController.h"
#include "core/Input.h"

OrthographicCameraController::OrthographicCameraController()
	:m_aspectRatio(1.0f), m_rotation(true)
{
}

OrthographicCameraController::~OrthographicCameraController()
{
}

void OrthographicCameraController::SetProjection(float aspectRatio, bool rotation)
{
	m_aspectRatio = aspectRatio;
	m_camera.SetProjection(-m_aspectRatio * m_zoom, m_aspectRatio * m_zoom, -m_zoom, m_zoom);
}

void OrthographicCameraController::OnEvent(Event& event)
{
	EventDispatcher dispatcher = EventDispatcher(event);
	dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&OrthographicCameraController::OnMouseScrolled, this, std::placeholders::_1));
	dispatcher.Dispatch<WindowResizedEvent>(std::bind(&OrthographicCameraController::OnWindowResized, this, std::placeholders::_1));
	dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&OrthographicCameraController::OnMouseButtonPressed, this, std::placeholders::_1));
	dispatcher.Dispatch<KeyPressedEvent>(std::bind(&OrthographicCameraController::OnEscPressed, this, std::placeholders::_1));
}

void OrthographicCameraController::OnUpdate(float timestep)
{
	glm::vec3 cameraPosition = m_camera.GetPosition();
	glm::vec3 cameraRotation = m_camera.GetRotation(); // pitch (x), yaw (y), roll (z)

	float yaw = glm::radians(cameraRotation.y);
	float pitch = glm::radians(cameraRotation.x);

	glm::vec3 forward = glm::normalize(glm::vec3(
		cos(pitch) * sin(yaw),
		sin(pitch),
		cos(pitch) * cos(yaw)
	));
	glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f))); // world up
	glm::vec3 up = glm::normalize(glm::cross(right, forward));

	if (Input::IsKeyPressed(GLFW_KEY_W))
		cameraPosition.y += m_cameraTranslationSpeed * timestep;
	if (Input::IsKeyPressed(GLFW_KEY_S))
		cameraPosition.y -= m_cameraTranslationSpeed * timestep;
	if (Input::IsKeyPressed(GLFW_KEY_A))
		cameraPosition -= right * m_cameraTranslationSpeed * timestep;
	if (Input::IsKeyPressed(GLFW_KEY_D))
		cameraPosition += right * m_cameraTranslationSpeed * timestep;

	std::pair<float, float> delta = Input::GetMousePositionDelta();

	if (m_mouseLocked) {
		float sensitivity = m_cameraRotationSpeed * timestep;
		m_camera.SetPosition(cameraPosition);

		cameraRotation.y += delta.first * sensitivity;
		cameraRotation.x += delta.second * sensitivity;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		m_camera.SetRotation(glm::vec3(pitch, yaw, 0.0f));
	}

	m_camera.SetPosition(cameraPosition);
	m_camera.SetRotation(cameraRotation);

	m_cameraTranslationSpeed = 3.0f * m_zoom;
}

void OrthographicCameraController::OnRender()
{
	m_camera.SetMVPMatrix();
}

bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
{
	m_zoom -= e.GetOffsetY() * 0.25f;
	m_zoom = std::max(m_zoom, 0.25f);
	m_camera.SetProjection(-m_aspectRatio * m_zoom, m_aspectRatio * m_zoom, -m_zoom, m_zoom);
	return false;
}

bool OrthographicCameraController::OnWindowResized(WindowResizedEvent& e)
{
	m_aspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
	m_camera.SetProjection(-m_aspectRatio * m_zoom, m_aspectRatio * m_zoom, -m_zoom, m_zoom);
	return false;
}

bool OrthographicCameraController::OnMouseButtonPressed(MouseButtonPressedEvent& e)
{
	if (e.GetButton() == GLFW_MOUSE_BUTTON_1) {
		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_mouseLocked = true;
	}
	return false;
}

bool OrthographicCameraController::OnEscPressed(KeyPressedEvent& e)
{
	if (e.GetKeyCode() == GLFW_KEY_ESCAPE) {
		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_mouseLocked = false;
	}
	return false;
}

