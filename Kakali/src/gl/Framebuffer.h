#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum FrameBufferAttachments {
	NONE = 0,
	COLOR = 1 << 0,
	DEPTH = 1 << 1,
	DEPTH_STENCIL = 1 << 2,
};

struct FrameBufferSpecification {
	uint32_t Width, Height;
	FrameBufferAttachments Attachments;
};

class FrameBuffer
{
public:
	FrameBuffer(const FrameBufferSpecification& props);
	~FrameBuffer();

	void Resize(uint32_t width, uint32_t height);
	void Invalidate();

	void Bind() const;
	void Unbind() const;

	inline uint32_t GetColorAttachment() const { return m_colorAttachment; }
	inline uint32_t GetDepthAttachment() const { return m_depthAttachment; }
private:
	uint32_t m_rendererID, m_colorAttachment, m_depthAttachment;
	FrameBufferSpecification m_spec;
};
