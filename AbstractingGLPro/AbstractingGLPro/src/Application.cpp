#include <iostream>
#include <fstream>
#include <string>
#include<sstream>
#include<GL/glew.h> /*Ҫ��dlfw֮ǰ������error*/
#include <GLFW/glfw3.h>
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"


// 7.ʹ��Uniform��ͨ��CPU��GPU����ֵ��������ɫֵ���Ա�����ɫ��֮�⶯̬�趨
void shaderWithUniform7(GLFWwindow* window){
	// ���������Σ�6����ά����ȥ���ص�����һ��������
	float position2[] = {
		-0.5f, -0.5f, //0
		0.5f, -0.5f,  //1
		0.5f, 0.5f,  //2

		-0.5f, 0.5f  //3
	};
	// ����
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

	// ��װShader���滻Ϊ���ã�
	Shader shader("res/shaders/Basic2.shader");
	shader.Bind();
	shader.setUniform4f("u_Color", 0.8f, 0.3f, 0.7f, 1.0f);

	// �滻Ϊ��
	va.UnBind();

	// �滻Ϊ��
	vb.UnBind();
	ib.UnBind();

	shader.UnBind();

	// �󶨶��㻺�����������䲼�֣�����ˣ��󶨸ö��������������������״̬
	// Ҳ����˵��֮ǰ�󶨶��㣬���ò��֣��������������ķ�ʽ����Ϊ�󶨶������飬��������������������Ⱦʱ���ҵ�������������Ȼ�����

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
		
		// ��whileѭ���иı��ɫɫֵ���γɶ���Ч��
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

	// ����GLFW���봴��һ��OpenGL������
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

	// ����Vsync�ź�ˢ��Ƶ�ʣ������Կ��ƶ�����Ҫ̫�죬�������Ĭ����0
	// ����������ˢ���ٶ�
	glfwSwapInterval(1);

	/* glew��ʼ��
	*/
	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	shaderWithUniform7(window);
	glfwTerminate();
	return 0;
}