#pragma once

#include<GL\glew.h>
#include <iostream>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); \
	x; \
	ASSERT(GLLogCall());
#define GLCall2(x) GLClearError(); \
	x; \
	ASSERT(GLLogCall2(#x, __FILE__, __LINE__))
// gl异常处理
void GLClearError();
void GLCheckError();
// 异常检测
bool GLLogCall();
// 异常检测
bool GLLogCall2(const char* function, const char* file, int line);

////////////////////////////////////////////////////////////////////////

class Renderer
{
public:
	void Clear() const;
	void Draw(const VertexArray& va,const IndexBuffer& ib, const Shader& shader) const;
};