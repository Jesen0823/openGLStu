#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"


VertexArray::VertexArray()
{
	// 将ID设置给顶点数组
	GLCall2(glGenVertexArrays(1, &m_RendererID));
	// 绑定顶点数组
	GLCall2(glBindVertexArray(m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall2(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::Bind() const
{
	GLCall2(glBindVertexArray(m_RendererID));
}

void VertexArray::UnBind() const
{
	GLCall2(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();

	// 设置缓冲区布局
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall2(glEnableVertexAttribArray(i));
		GLCall2(glVertexAttribPointer(i, element.count, element.type, 
			element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}