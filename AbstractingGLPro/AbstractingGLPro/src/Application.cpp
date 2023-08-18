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

// 7.使用Uniform来通过CPU向GPU传递值，比如颜色值，以便在着色器之外动态设定
void handleOpenGl(GLFWwindow* window){
	// 两个三角形，6个二维顶点去掉重叠，画一个正方形
	float position2[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, //0
		0.5f,  -0.5f, 1.0f, 0.0f, //1
		0.5f,  0.5f,  1.0f, 1.0f, //2

		-0.5f, 0.5f,  0.0f, 1.0f  //3
	};
	// 索引
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	// 透明度的混合
	GLCall2(glEnable(GL_BLEND));
	GLCall2(glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA));

	VertexArray va;
	VertexBuffer vb(position2, 4 * 4 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	va.AddBuffer(vb,layout);

	IndexBuffer ib(indices, 6);

	Shader shader("res/shaders/Basic3.shader");
	shader.Bind();
	shader.setUniform4f("u_Color", 0.8f, 0.3f, 0.7f, 1.0f);

	Texture texture("res/logoimg.png");
	texture.Bind();
	shader.setUniform1i("u_Texture", 0);

	va.UnBind();
	vb.UnBind();
	ib.UnBind();
	shader.UnBind();

	Renderer renderer;

	float r = 0.0f;
	float increment = 0.05f;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		renderer.Clear();

		shader.Bind();
		shader.setUniform4f("u_Color", r, 0.5f, 0.8f, 1.0f);
		renderer.Draw(va,ib,shader);
		
		/* 在while循环中改变红色色值，形成动画效果 */
		if (r > 1.0f)
			increment = -0.05f;
		else if (r < 0.0f)
			increment = 0.05f;
		r += increment;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
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
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
	glfwTerminate();
	return 0;
}