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
#include "Texture.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw_gl3.h"

// 7.ʹ��Uniform��ͨ��CPU��GPU����ֵ��������ɫֵ���Ա�����ɫ��֮�⶯̬�趨
void handleOpenGl(GLFWwindow* window){
	// ���������Σ�6����ά����ȥ���ص�����һ��������
	float position2[] = {
		100.0f, 100.0f, 0.0f, 0.0f, //0
		200.0f,  100.0f, 1.0f, 0.0f, //1
		200.0f,  200.0f,  1.0f, 1.0f, //2

		100.0f, 200.0f, 0.0f, 1.0f  //3
	};
	// ����
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	// ͸���ȵĻ��
	GLCall2(glEnable(GL_BLEND));
	GLCall2(glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA));

	VertexArray va;
	VertexBuffer vb(position2, 4 * 4 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	va.AddBuffer(vb,layout);

	IndexBuffer ib(indices, 6);

	/* ��������ͶӰ���� ����ͶӰ, ����Ե,�Ҳ��Ե,�ײ���Ե,������Ե,��ƽ��,Զƽ�� */
	glm::mat4 proj = glm::ortho(-0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	//glm::vec4 vp(100.0f, 100.0f, 0.0f, 1.0f);
	//glm::vec4 result = proj * vp;
	
	 // MVP ģ����ͼͶӰ���󣬾������  
	// ģ��model����ͼ����view matrix��ͬ����ͼ�����������ͼ��һ��
	// ��ͼ������������λ�úͷ���
	// ģ�;������۲����
	// ����任����ƽ����ת���ŵ�ת��


	// ��ͼ���� glm::mat4(1.0f) ����һ����λ����
	// �������100����Ч������Եģ���������100
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
	

	Shader shader("res/shaders/Basic3.shader");
	shader.Bind();
	shader.setUniform4f("u_Color", 0.8f, 0.3f, 0.7f, 1.0f);
	//shader.setUniformMat4f("u_MVP", proj);

	Texture texture("res/demo.png");
	texture.Bind();
	shader.setUniform1i("u_Texture", 0);

	va.UnBind();
	vb.UnBind();
	ib.UnBind();
	shader.UnBind();

	Renderer renderer;

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	// Setup style
	ImGui::StyleColorsDark();

	glm::vec3 translation(200, 200, 0);

	float r = 0.0f;
	float increment = 0.05f;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		renderer.Clear();

		ImGui_ImplGlfwGL3_NewFrame();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
		// Ϊmvp���󴴽�һ��glmӳ��
		glm::mat4 mvp = proj * view * model;

		shader.Bind();
		shader.setUniform4f("u_Color", r, 0.5f, 0.8f, 1.0f);
		shader.setUniformMat4f("u_MVP", mvp);

		renderer.Draw(va,ib,shader);
		
		/* ��whileѭ���иı��ɫɫֵ���γɶ���Ч�� */
		if (r > 1.0f)
			increment = -0.05f;
		else if (r < 0.0f)
			increment = 0.05f;

		r += increment;

		{
			ImGui::SliderFloat("Translation", &translation.x, 0.0f, 960.0f);    
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

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

	/* ����GLFW���봴��һ��OpenGL������ */
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

	/* ����Vsync�ź�ˢ��Ƶ�ʣ������Կ��ƶ�����Ҫ̫�죬�������Ĭ����0,����������ˢ���ٶ� */
	glfwSwapInterval(1);

	/* glew��ʼ�� */
	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	handleOpenGl(window);
	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}