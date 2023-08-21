#include <iostream>
#include <fstream>
#include <string>
#include<sstream>
#include<GL/glew.h> /*要在dlfw之前，否则error*/
#include <GLFW/glfw3.h>
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw_gl3.h"

#include "test\TestClearColor.h"
#include "test\TestTexture2D.h"
#include "test\TestBatchRender.h"

// 7.使用Uniform来通过CPU向GPU传递值，比如颜色值，以便在着色器之外动态设定
void handleOpenGl(GLFWwindow* window){

	// 透明度的混合
	GLCall2(glEnable(GL_BLEND));
	GLCall2(glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA));

	Renderer renderer;

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	// Setup style
	ImGui::StyleColorsDark();

	test::Test* currentTest = nullptr;;
	test::TestMenu* testMenu = new test::TestMenu(currentTest);
	currentTest = testMenu;

	testMenu->RegisterTest<test::TestClearColor>("Clear Color");
	testMenu->RegisterTest<test::TestTexture2D>("texture 2D");
	testMenu->RegisterTest<test::TestBatchRender>("batch Render");
	
	test::TestClearColor test;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		GLCall2(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		renderer.Clear();

		ImGui_ImplGlfwGL3_NewFrame();
		
		if (currentTest)
		{
			currentTest->OnUpdate(0.0f);
			currentTest->OnRender();
			ImGui::Begin("Test");
			if (currentTest != testMenu && ImGui::Button("<-"))
			{
				delete currentTest;
				currentTest = testMenu;
			}
			currentTest->OnImGuiRender();
			ImGui::End();
		}

		test.OnImGuiRender();

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	delete currentTest;
	if (currentTest != testMenu)
	{
		delete testMenu;
	}
}
int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* 告诉GLFW我想创建一个OpenGL上下文 */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_COMPAT_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* 设置Vsync信号刷新频率，降低以控制动画不要太快，如果不设默认是0,采用它控制刷新速度 */
	glfwSwapInterval(1);

	/* glew初始化 */
	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	handleOpenGl(window);
	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}