#pragma once

#include "pch.h"
#include "layers/Layer.h"
#include "gl/PerspectiveCameraController.h"
#include "gl/Shader.h"
#include "gl/VertexArray.h"
#include "gl/VertexBuffer.h"
#include "gl/VertexBufferLayout.h"
#include "gl/Skybox.h"

class SandboxLayer : public Layer
{
public:
	SandboxLayer();

	void OnAttach() override;
	void OnEvent(Event& evnet) override;
	void OnUpdate(float timestep) override;
	void OnRender() override;
	void OnImGuiRender() override;
	void OnDetach() override;
private:
	PerspectiveCameraController m_camera;
	Shader m_skyboxShader;
	VertexArray m_vao;
	Skybox m_skybox;
	std::shared_ptr<VertexBuffer> p_vbo;
};

