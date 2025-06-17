#include "pch.h"

#include "OrthographicCamera.h"
#include "core/Input.h"
#include "core/Window.h"


OrthographicCamera::OrthographicCamera()
	:m_position(glm::vec3(0.0f)), m_rotation(0.0f), m_viewMatrix(glm::mat4(1.0f)),
	 m_aspect(1.0f), m_projectionMatrix(glm::mat4(1.0f)), m_viewProjectionMatrix(glm::mat4(1.0f)),
	 m_zoom(1.0f)
{
}

void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
{
	m_projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 100.0f);
	m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;

	m_zoom = top;
	m_aspect = right / top;
}

void OrthographicCamera::SetMVPMatrix()
{
	//TODO:
}

void OrthographicCamera::SetPosition(const glm::vec3& position)
{
	m_position = position;
	RecalculateViewMatrix();
}

void OrthographicCamera::SetRotation(glm::vec3 rotation)
{
	m_rotation = rotation;
	RecalculateViewMatrix();
}

const glm::vec2 OrthographicCamera::GetMousePositionWorldSpace() const
{
	auto [mx, my] = Input::GetMousePosition();
	const auto& windowData = Window::GetGlobalWindowData();

	my = windowData.Height - my;

	float ndcX = (2.0f * mx) / windowData.Width - 1.0f;
	float ndcY = (2.0f * my) / windowData.Height - 1.0f;

	glm::vec4 clipCoords = glm::vec4(ndcX, ndcY, 0.0f, 1.0f); 

	glm::mat4 invVP = glm::inverse(m_viewProjectionMatrix);
	glm::vec4 worldCoords = invVP * clipCoords;

	return glm::vec3(worldCoords);
}

void OrthographicCamera::RecalculateViewMatrix()
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position);

	transform = glm::rotate(transform, glm::radians(m_rotation.x), glm::vec3(1, 0, 0)); // pitch
	transform = glm::rotate(transform, glm::radians(m_rotation.y), glm::vec3(0, 1, 0)); // yaw
	transform = glm::rotate(transform, glm::radians(m_rotation.z), glm::vec3(0, 0, 1)); // roll

	m_viewMatrix = glm::inverse(transform);
	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

