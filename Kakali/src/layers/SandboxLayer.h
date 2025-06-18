#pragma once

#include "pch.h"
#include "layers/Layer.h"
#include "gl/PerspectiveCameraController.h"
#include "gl/Shader.h"
#include "gl/VertexArray.h"
#include "gl/VertexBuffer.h"
#include "gl/VertexBufferLayout.h"
#include "gl/Skybox.h"
#include "gl/Model.h"
#include "gl/Texture.h"
#include "gl/OrthographicCameraController.h"
#include "gl/Framebuffer.h"

struct CameraDirection {
	uint32_t ID;
	glm::vec3 Direction;
	glm::vec3 Up;
};

class SandboxLayer : public Layer
{
public:
	void OnAttach() override;
	void OnEvent(Event& evnet) override;
	void OnUpdate(float timestep) override;
	void OnRender() override;
	void OnImGuiRender() override;
	void OnDetach() override;
private:
	PerspectiveCameraController m_camera;
	PerspectiveCamera m_spotCamera, m_pointCamera;
	OrthographicCamera m_shadowOrtho;
	Shader m_skyboxShader, m_lightingShader, m_shadowShader;
	VertexArray m_vao;
	Skybox m_skybox;
	std::shared_ptr<VertexBuffer> p_vbo;
	Model m_backpack;
	Mesh m_floor;
	Texture m_floorTexture;
	FrameBuffer m_dirShadowMap, m_spotShadowMap, m_pointShadowMap;

	glm::vec3 globalAmbient = glm::vec3(0.5f, 0.5f, 0.8f);
	float globalIntensity = 1.0f;

	glm::vec3 dirAmbient = glm::vec3(0.0f, 1.0f, 1.0f);
	glm::vec3 dirDiffuse = glm::vec3(1.0f);
	glm::vec3 dirSpecular = glm::vec3(1.0f);
	glm::vec3 dirDirection = glm::vec3(-50.0f, 20.0f, 0.0f);
	float dirIntensity = 0.66f;

	glm::vec3 spotPosition = glm::vec3(-1.55f, 6.0f, 0.0f);
	glm::vec3 spotAmbient = glm::vec3(0.7f, 0.0f, 1.0f);
	glm::vec3 spotDiffuse = glm::vec3(1.0f);
	glm::vec3 spotSpecular = glm::vec3(1.0f);
	glm::vec3 spotDirection = glm::vec3(87.0f, 0.0f, 0.0f);
	float spotIntensity = 0.66f;
	float spotAngle = 20.0f;
	float spotOuterAngle = 30.0f;

	glm::vec3 pointPosition = glm::vec3(2.4f, -2.1f, 1.5f);
	glm::vec3 pointAmbient = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 pointDiffuse = glm::vec3(1.0f);
	glm::vec3 pointSpecular = glm::vec3(1.0f);
	float pointIntensity = 0.9f;
	float pointRadius = 4.0f;

	uint32_t m_pointCubemapID;

	CameraDirection m_cameraDirections[6] =
	{
		{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f) },
		{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) },
		{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y, glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 0.0f, -1.0f) },
		{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
		{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z, glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f) },
		{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f) }
	};
};

