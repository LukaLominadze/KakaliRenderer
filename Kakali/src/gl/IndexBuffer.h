#pragma once

#include "pch.h"

class IndexBuffer
{
public:
	IndexBuffer() = default;
	~IndexBuffer() = default;

	void GenBuffer(const void* data, uint32_t count);
	void Delete();

	void Bind() const;
	void Unbind() const;

	void SetData(const void* data, uint32_t count);

	inline const uint32_t GetRendererID() const { return m_rendererID; }
	inline uint32_t GetCount() const { return m_count; }
private:
	uint32_t m_rendererID = 0;
	uint32_t m_count = 0;
};


