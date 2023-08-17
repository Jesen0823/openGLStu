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

/*struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};*/

/*static ShaderProgramSource ParseShader(const std::string& filePath)
{

	enum  class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};


	std::ifstream stream(filePath);
	std::string line;
	ShaderType shaderType = ShaderType::NONE;
	std::stringstream ss[2];
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				shaderType = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				shaderType = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)shaderType] << line << "\n";
		}
	}
	return{ ss[0].str(), ss[1].str() };
}*/

/** 
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	//const char* src = &source[0];
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// Error handing
	int ret;
	glGetShaderiv(id, GL_COMPILE_STATUS, &ret);
	if (ret == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length* sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader." << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}
**/

/*static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}*/

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

	// 替换以下：
	//unsigned int vao;
	//GLCall2(glGenVertexArrays(1,&vao)); // 将ID设置给顶点数组
	//GLCall2(glBindVertexArray(vao)); // 绑定顶点数组
	// 到 VertexArray::Bind()

	VertexArray va;
	VertexBuffer vb(position2, 4 * 2 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(2);
	va.AddBuffer(vb,layout);

	IndexBuffer ib(indices, 6);

	// 替换以下：
	/*unsigned int buffer;
	GLCall2(glGenBuffers(1, &buffer));
	GLCall2(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	GLCall2(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), position2, GL_STATIC_DRAW));*/
	// 为：
	//VertexBuffer vb(position2, 4 * 2 * sizeof(float));

	// 替换以下布局绑定：
	/*glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)* 2, 0);*/
	// 到 VertexArray::AddBuffer

	// 替换以下：
	/*unsigned int ibo;
	GLCall2(glGenBuffers(1, &ibo));
	GLCall2(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall2(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));*/
	// 为：
	//IndexBuffer ib(indices, 6);

	// 以下移入Shader.cpp
	/*ShaderProgramSource source = ParseShader("res/shaders/Basic2.shader");
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	GLCall2(glUseProgram(shader));
	GLCall2(int location = glGetUniformLocation(shader, "u_Color"));
	ASSERT(location != -1);
	GLCall2(glUniform4f(location, 0.2f, 0.5f, 0.8f, 1.0f));
	*/
	// 封装Shader后替换为调用：
	Shader shader("res/shaders/Basic2.shader");
	shader.Bind();
	shader.setUniform4f("u_Color", 0.8f, 0.3f, 0.7f, 1.0f);

	// 添加清除绑定逻辑
	// GLCall2(glBindVertexArray(0));
	// 替换为：
	va.UnBind();

	//GLCall2(glBindBuffer(GL_ARRAY_BUFFER,0));
	//GLCall2(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	// 替换为：
	vb.UnBind();
	ib.UnBind();

	// GLCall2(glUseProgram(0));
	// 替换为：
	shader.UnBind();

	// 绑定顶点缓冲区并设置其布局，变成了，绑定该顶点数组对象，它包含所有状态
	// 也就是说，之前绑定顶点，设置布局，绑定索引缓冲区的方式，改为绑定顶点数组，绑定索引缓冲区，后面渲染时，找到索引缓冲区，然后绘制

	Renderer renderer;


	float r = 0.0f;
	float increment = 0.05f;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		//GLCall2(glClear(GL_COLOR_BUFFER_BIT));
		///// 或者 直接在Renderer处理
		renderer.Clear();

		// 添加重新绑定
		//GLCall2(glUseProgram(shader));
		// 循环重绘，形成动画
		//GLCall2(glUniform4f(location, r, 0.5f, 0.8f, 1.0f));
		// 替换为：
		shader.Bind();
		shader.setUniform4f("u_Color", r, 0.5f, 0.8f, 1.0f);

		renderer.Draw(va,ib,shader);
		
		// 替换以下：
		//GLCall2(glBindVertexArray(vao));  // 绑定顶点数组
		// 为：
		//va.Bind();
		///// 或者 直接在Renderer处理

		// 替换以下：
		//GLCall2(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
		// 为:
		//ib.Bind();
		///// 或者 直接在Renderer处理

		// glDrawElements是与索引缓冲区一起使用的
		//GLCall2(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));// 无异常代码
		///// 或者 直接在Renderer处理

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
	// glDeleteProgram(shader); // 迁往Shader析构函数中
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