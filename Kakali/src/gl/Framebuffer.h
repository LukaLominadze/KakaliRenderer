#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum class FrameBufferAttachments {
	NONE = 0,
	COLOR = 1 << 0,
	DEPTH = 1 << 1,
	DEPTH_STENCIL = 1 << 2,
};

inline FrameBufferAttachments operator|(FrameBufferAttachments a, FrameBufferAttachments b) {
	return static_cast<FrameBufferAttachments>(
		static_cast<int>(a) | static_cast<int>(b));
}

inline FrameBufferAttachments operator&(FrameBufferAttachments lhs, FrameBufferAttachments rhs) {
	return static_cast<FrameBufferAttachments>(
		static_cast<int>(lhs) & static_cast<int>(rhs));
}

struct FrameBufferSpecification {
	uint32_t Width, Height;
	FrameBufferAttachments Attachments;
};

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer() = default;

	void GenBuffer(const FrameBufferSpecification& props);
	void Delete();

	void Resize(uint32_t width, uint32_t height);
	void Invalidate();

	void Bind() const;
	void Unbind() const;

	inline uint32_t GetColorAttachment() const { return m_colorAttachment; }
	inline uint32_t GetDepthAttachment() const { return m_depthAttachment; }

	inline const uint32_t GetRendererID() const { return m_rendererID; }
private:
	uint32_t m_rendererID, m_colorAttachment, m_depthAttachment;
	FrameBufferSpecification m_spec;
};
