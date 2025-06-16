#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

class Skybox
{
public:
	Skybox() = default;
	~Skybox() = default;

	void GenTexture(const std::string& filePath);
	void Delete();

	void Bind() const;
	void Unbind() const;

	inline const uint32_t GetRendererID() const { return m_rendererID; }
private:
	uint32_t m_rendererID;
};

