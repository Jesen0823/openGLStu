#include "Renderer.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

void GLCheckError()
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
	}
}

bool GLLogCall()
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL ERROR] (" << error << ")" << std::endl;
		return false;
	}
	return true;
}

bool GLLogCall2(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL ERROR location] (" << error << std::endl;
		std::cout << "function: " << function << ", file: " << file << ", line: " << line << ")" << std::endl;
		return false;
	}
	return true;
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	GLCall2(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
	GLCall2(glClear(GL_COLOR_BUFFER_BIT));
}