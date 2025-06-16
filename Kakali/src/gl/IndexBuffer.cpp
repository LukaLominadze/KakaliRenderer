#include "pch.h"

#include "IndexBuffer.h"

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::SetData(const void* data, uint32_t count)
{
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(uint32_t), data, GL_DYNAMIC_DRAW));
}

void IndexBuffer::GenBuffer(const void* data, uint32_t count)
{
    ASSERT(sizeof(uint32_t) == sizeof(GLuint), "Checking if GLuint int is 4 bytes...");

    m_count = count;

    GLCall(glGenBuffers(1, &m_rendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(uint32_t), data, GL_DYNAMIC_DRAW));
}

void IndexBuffer::Delete()
{
    if (m_rendererID == -1) {
        GLCall(glDeleteBuffers(1, &m_rendererID));
        m_rendererID = -1;
    }
}

