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
	OrthographicCamera m_shadowOrtho;
	Shader m_skyboxShader, m_lightingShader, m_shadowShader;
	VertexArray m_vao;
	Skybox m_skybox;
	std::shared_ptr<VertexBuffer> p_vbo;
	Model m_backpack;
	Mesh m_floor;
	Texture m_floorTexture;
	FrameBuffer m_shadowMap;

	glm::vec3 globalAmbient = glm::vec3(1.0f);
	float globalIntensity = 1.0f;

	glm::vec3 dirAmbient = glm::vec3(1.0f);
	glm::vec3 dirDiffuse = glm::vec3(1.0f);
	glm::vec3 dirSpecular = glm::vec3(1.0f);
	glm::vec3 dirDirection = glm::vec3(-50.0f, 20.0f, 0.0f);
	float dirIntensity = 1.0f;
};

