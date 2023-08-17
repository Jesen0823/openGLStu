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