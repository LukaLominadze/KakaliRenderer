#pragma once

#include "pch.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
public:
	VertexArray() = default;
	~VertexArray() = default;

	void GenBuffer();
	void Delete();

	void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vbo, const VertexBufferLayout& vboLayout);
	void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& ibo);

	void Bind() const;
	void Unbind() const;

	void SetVertexBufferData(const void* data, uint32_t size);
	void SetIndexBufferData(const void* data, uint32_t count);

	inline const uint32_t GetRendererID() const { return m_rendererID; }
private:
	uint32_t m_rendererID = 0;

	std::shared_ptr<VertexBuffer> r_vbo;
	std::shared_ptr<IndexBuffer> r_ibo;
};

