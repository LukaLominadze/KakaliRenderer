#pragma once

#include "pch.h"

class Texture
{
public:
	Texture();
	~Texture() = default;

	void GenTexture(const std::string& filePath);
	void Delete();

	void Bind(uint32_t slot) const;
	void Unbind() const;

	inline int GetWidth() const { return m_width; }
	inline int GetHeight() const { return m_height; }

	inline const uint32_t GetRendererID() const { return m_rendererID; }
private:
	uint32_t m_rendererID;
	std::string m_filePath;
	unsigned char* m_localBuffer;
	int m_width, m_height, m_BPP;
};


