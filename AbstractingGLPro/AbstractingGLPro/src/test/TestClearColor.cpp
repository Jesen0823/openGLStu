#include "TestClearColor.h"

#include <GL/glew.h>
#include "Renderer.h"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw_gl3.h"

namespace test{

	TestClearColor::TestClearColor()
	{
		m_ClearColor[0] = 0.2f;
		m_ClearColor[1] = 0.3f;
		m_ClearColor[2] = 0.8f;
		m_ClearColor[3] = 1.0f;
	}
	TestClearColor::~TestClearColor()
	{

	}

	void TestClearColor::OnUpdate(float deltaTime)
	{

	}
	void TestClearColor::OnRender()
	{
		GLCall2(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		GLCall2(glClear(GL_COLOR_BUFFER_BIT));
	}
	void TestClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
	}
}