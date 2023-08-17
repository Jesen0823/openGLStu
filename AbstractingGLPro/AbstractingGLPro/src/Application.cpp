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


// 7.使用Uniform来通过CPU向GPU传递值，比如颜色值，以便在着色器之外动态设定
void shaderWithUniform7(GLFWwindow* window){
	// 两个三角形，6个二维顶点去掉重叠，画一个正方形
	float position2[] = {
		-0.5f, -0.5f, //0
		0.5f, -0.5f,  //1
		0.5f, 0.5f,  //2

		-0.5f, 0.5f  //3
	};
	// 索引
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	VertexArray va;
	VertexBuffer vb(position2, 4 * 2 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(2);
	va.AddBuffer(vb,layout);

	IndexBuffer ib(indices, 6);

	// 封装Shader后替换为调用：
	Shader shader("res/shaders/Basic2.shader");
	shader.Bind();
	shader.setUniform4f("u_Color", 0.8f, 0.3f, 0.7f, 1.0f);

	// 替换为：
	va.UnBind();

	// 替换为：
	vb.UnBind();
	ib.UnBind();

	shader.UnBind();

	// 绑定顶点缓冲区并设置其布局，变成了，绑定该顶点数组对象，它包含所有状态
	// 也就是说，之前绑定顶点，设置布局，绑定索引缓冲区的方式，改为绑定顶点数组，绑定索引缓冲区，后面渲染时，找到索引缓冲区，然后绘制

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
		
		// 在while循环中改变红色色值，形成动画效果
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

	// 告诉GLFW我想创建一个OpenGL上下文
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

	// 设置Vsync信号刷新频率，降低以控制动画不要太快，如果不设默认是0
	// 采用它控制刷新速度
	glfwSwapInterval(1);

	/* glew初始化
	*/
	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	shaderWithUniform7(window);
	glfwTerminate();
	return 0;
}