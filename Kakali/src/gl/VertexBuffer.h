#pragma once

#include "pch.h"

class VertexBuffer
{
public:
	VertexBuffer() = default;
	~VertexBuffer() = default;

	void GenBuffer(const void* data, uint32_t size);
	void Delete();

	void SetData(const void* data, uint32_t size);

	void Bind() const;
	void Unbind() const;

	inline const uint32_t GetRendererID() const { return m_rendererID; }
private:
	uint32_t m_rendererID = 0;
};


