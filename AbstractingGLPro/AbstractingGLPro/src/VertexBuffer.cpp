#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GLCall2(glGenBuffers(1, &m_RendererID));
	GLCall2(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall2(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	// É¾³ý¶¥µã»º´æÇø
	GLCall2(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	GLCall2(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::UnBind() const
{
	GLCall2(glBindBuffer(GL_ARRAY_BUFFER, 0));
}