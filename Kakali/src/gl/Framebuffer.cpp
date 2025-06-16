#include "Framebuffer.h"
#include <iostream>

#include "core/Log.h"

FrameBuffer::FrameBuffer()
	:m_rendererID(0), m_colorAttachment(0), m_depthAttachment(0)
{
}

void FrameBuffer::GenBuffer(const FrameBufferSpecification& props)
{
	GLCall(glCreateFramebuffers(1, &m_rendererID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID));

	if ((m_spec.Attachments & FrameBufferAttachments::COLOR) == FrameBufferAttachments::COLOR) {
		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_colorAttachment));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_colorAttachment));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_spec.Width, m_spec.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0));
	}
	if ((m_spec.Attachments & FrameBufferAttachments::DEPTH) == FrameBufferAttachments::DEPTH) {
		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachment));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_depthAttachment));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_spec.Width, m_spec.Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0));
	}
	if ((m_spec.Attachments & FrameBufferAttachments::DEPTH_STENCIL) == FrameBufferAttachments::DEPTH_STENCIL) {
		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachment));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_depthAttachment));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_spec.Width, m_spec.Height, 0, GL_DEPTH_STENCIL,
			GL_UNSIGNED_INT_24_8, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0));
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer complete!" << std::endl;
	}

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::Delete()
{
	if ((m_spec.Attachments & FrameBufferAttachments::COLOR) == FrameBufferAttachments::COLOR)
		GLCall(glDeleteTextures(1, &m_colorAttachment));
	if ((m_spec.Attachments & FrameBufferAttachments::DEPTH) == FrameBufferAttachments::DEPTH ||
		(m_spec.Attachments & FrameBufferAttachments::DEPTH_STENCIL) == FrameBufferAttachments::DEPTH_STENCIL)
		GLCall(glDeleteTextures(1, &m_depthAttachment));
	GLCall(glDeleteFramebuffers(1, &m_rendererID));
}

void FrameBuffer::Resize(uint32_t width, uint32_t height)
{
	m_spec.Width = width;
	m_spec.Height = height;

	Invalidate();
}

void FrameBuffer::Invalidate()
{
	if (m_rendererID) {
		if ((m_spec.Attachments & FrameBufferAttachments::COLOR) == FrameBufferAttachments::COLOR)
			GLCall(glDeleteTextures(1, &m_colorAttachment));
		if ((m_spec.Attachments & FrameBufferAttachments::DEPTH) == FrameBufferAttachments::DEPTH ||
			(m_spec.Attachments & FrameBufferAttachments::DEPTH_STENCIL) == FrameBufferAttachments::DEPTH_STENCIL)
			GLCall(glDeleteTextures(1, &m_depthAttachment));
		GLCall(glDeleteFramebuffers(1, &m_rendererID));
	}

	GLCall(glCreateFramebuffers(1, &m_rendererID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID));

	if ((m_spec.Attachments & FrameBufferAttachments::COLOR) == FrameBufferAttachments::COLOR) {
		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_colorAttachment));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_colorAttachment));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_spec.Width, m_spec.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0));
	}
	if ((m_spec.Attachments & FrameBufferAttachments::DEPTH) == FrameBufferAttachments::DEPTH) {
		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachment));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_depthAttachment));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_spec.Width, m_spec.Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0));
	}
	if ((m_spec.Attachments & FrameBufferAttachments::DEPTH_STENCIL) == FrameBufferAttachments::DEPTH_STENCIL) {
		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachment));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_depthAttachment));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_spec.Width, m_spec.Height, 0, GL_DEPTH_STENCIL,
			GL_UNSIGNED_INT_24_8, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0));
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer not complete!" << std::endl;
	}

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::Bind() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID));
}

void FrameBuffer::Unbind() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}