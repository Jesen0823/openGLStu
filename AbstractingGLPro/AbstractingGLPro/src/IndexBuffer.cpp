#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) :m_Count(count)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCall2(glGenBuffers(1, &m_RendererID));
	GLCall2(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCall2(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
	//GLCall2(glBufferData(GL_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	// É¾³ý¶¥µã»º´æÇø
	GLCall2(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
	GLCall2(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::UnBind() const
{
	GLCall2(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}