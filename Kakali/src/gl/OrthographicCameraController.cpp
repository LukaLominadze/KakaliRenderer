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
	m_camera.SetProjection(-m_aspectRatio * m_zoom, m_aspectRatio * m_zoom, -m_zoom, m_zoom);
}

void OrthographicCameraController::OnEvent(Event& event)
{
	EventDispatcher dispatcher = EventDispatcher(event);
	dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&OrthographicCameraController::OnMouseScrolled, this, std::placeholders::_1));
	dispatcher.Dispatch<WindowResizedEvent>(std::bind(&OrthographicCameraController::OnWindowResized, this, std::placeholders::_1));
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

	if (Input::IsKeyPressed(GLFW_KEY_W))
		cameraPosition += forward * m_cameraTranslationSpeed * timestep;
	if (Input::IsKeyPressed(GLFW_KEY_S))
		cameraPosition -= forward * m_cameraTranslationSpeed * timestep;
	if (Input::IsKeyPressed(GLFW_KEY_A))
		cameraPosition -= right * m_cameraTranslationSpeed * timestep;
	if (Input::IsKeyPressed(GLFW_KEY_D))
		cameraPosition += right * m_cameraTranslationSpeed * timestep;

	if (Input::IsKeyPressed(GLFW_KEY_Q))
		cameraRotation.y += m_cameraRotationSpeed * timestep;
	if (Input::IsKeyPressed(GLFW_KEY_E))
		cameraRotation.y -= m_cameraRotationSpeed * timestep;

	if (Input::IsKeyPressed(GLFW_KEY_R))
		cameraRotation.x += m_cameraRotationSpeed * timestep;
	if (Input::IsKeyPressed(GLFW_KEY_F))
		cameraRotation.x -= m_cameraRotationSpeed * timestep;

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


