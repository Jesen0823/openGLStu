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

// 7.使用Uniform来通过CPU向GPU传递值，比如颜色值，以便在着色器之外动态设定
void handleOpenGl(GLFWwindow* window){
	// 两个三角形，6个二维顶点去掉重叠，画一个正方形
	float position2[] = {
		100.0f, 100.0f, 0.0f, 0.0f, //0
		200.0f,  100.0f, 1.0f, 0.0f, //1
		200.0f,  200.0f,  1.0f, 1.0f, //2

		100.0f, 200.0f, 0.0f, 1.0f  //3
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

	/* 正交矩阵，投影矩阵 处理投影, 左侧边缘,右侧边缘,底部边缘,顶部边缘,近平面,远平面 */
	glm::mat4 proj = glm::ortho(-0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	//glm::vec4 vp(100.0f, 100.0f, 0.0f, 1.0f);
	//glm::vec4 result = proj * vp;
	
	 // MVP 模型视图投影矩阵，矩阵相乘  
	// 模型model和视图矩阵view matrix不同，视图矩阵是相机视图的一种
	// 视图矩阵代表相机的位置和方向
	// 模型矩阵代表观察对象
	// 矩阵变换，即平移旋转缩放的转换


	// 视图矩阵 glm::mat4(1.0f) 构造一个单位矩阵
	// 相机左移100，则效果是相对的，物体右移100
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
		// 为mvp矩阵创建一个glm映射
		glm::mat4 mvp = proj * view * model;

		shader.Bind();
		shader.setUniform4f("u_Color", r, 0.5f, 0.8f, 1.0f);
		shader.setUniformMat4f("u_MVP", mvp);

		renderer.Draw(va,ib,shader);
		
		/* 在while循环中改变红色色值，形成动画效果 */
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
	glfwTerminate();
	return 0;
}