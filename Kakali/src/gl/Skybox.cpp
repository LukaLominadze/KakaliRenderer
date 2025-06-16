#include "Skybox.h"

#include <array>
#include <stb_image.h>
#include "pch.h"

Skybox::Skybox(const std::string& filePath)
{
	std::array<std::string, 6> faces = {
		"right.jpg", 
		"left.jpg",  
		"top.jpg",   
		"bottom.jpg",
		"front.jpg", 
		"back.jpg"   
	};

	GLCall(glGenTextures(1, &m_rendererID));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererID));

	int width, height, nrChannels;
	unsigned char* data;
	std::string _filePath = filePath + "/";
	for (unsigned int i = 0; i < faces.size(); i++) {
		_filePath += faces[i];

		data = stbi_load(_filePath.c_str(), &width, &height, &nrChannels, 0);
		if (!data) {
			std::cerr << "Failed to load cubemap face: " << _filePath << std::endl;
			_filePath = filePath + "/";
			continue; // or return;
		}
		GLCall(glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
		));

		_filePath = filePath + "/";
	}
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
}

Skybox::~Skybox()
{
	if (m_rendererID != -1) {
		GLCall(glDeleteTextures(1, &m_rendererID));
		m_rendererID = -1;
	}
}

void Skybox::Bind() const
{
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererID));
}

void Skybox::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}
