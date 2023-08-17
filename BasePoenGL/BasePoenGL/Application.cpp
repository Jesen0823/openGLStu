#include <iostream>
#include <fstream>
#include <string>
#include<sstream>
#include<GL/glew.h> /*Ҫ��dlfw֮ǰ������error*/
#include <GLFW/glfw3.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); \
	x; \
	ASSERT(GLLogCall());
#define GLCall2(x) GLClearError(); \
	x; \
	ASSERT(GLLogCall2(#x, __FILE__, __LINE__))
// gl�쳣����
static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}
static void GLCheckError()
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
	}
}
// �쳣���
static bool GLLogCall()
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL ERROR] (" << error << ")" << std::endl;
		return false;
	}
	return true;
}
// �쳣���
static bool GLLogCall2(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL ERROR location] (" << error << std::endl;
		std::cout << "function: " << function << ", file: " << file << ", line: " << line << ")" << std::endl;
		return false;
	}
	return true;
}
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filePath)
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
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	/*const char* src = &source[0];*/
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

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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
}

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

	unsigned int vao;
	// ��ID���ø���������
	GLCall2(glGenVertexArrays(1,&vao));
	// �󶨶�������
	GLCall2(glBindVertexArray(vao));

	unsigned int buffer;
	GLCall2(glGenBuffers(1, &buffer));
	GLCall2(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	GLCall2(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), position2, GL_STATIC_DRAW));

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)* 2, 0);

	unsigned int ibo;
	GLCall2(glGenBuffers(1, &ibo));
	GLCall2(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall2(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

	// ��ɫ�������ļ���
	ShaderProgramSource source = ParseShader("res/shaders/Basic2.shader");
	std::cout << "Vertex: " << std::endl;
	std::cout << source.VertexSource << std::endl;
	std::cout << "Fragment: " << std::endl;
	std::cout << source.FragmentSource << std::endl;
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	
	GLCall2(glUseProgram(shader));

	GLCall2(int location = glGetUniformLocation(shader, "u_Color"));
	ASSERT(location != -1);
	GLCall2(glUniform4f(location, 0.2f, 0.5f, 0.8f, 1.0f));

	// ���������
	GLCall2(glBindVertexArray(0));
	GLCall2(glUseProgram(0));
	GLCall2(glBindBuffer(GL_ARRAY_BUFFER,0));
	GLCall2(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	// �󶨶��㻺�����������䲼�֣�����ˣ��󶨸ö��������������������״̬
	// Ҳ����˵��֮ǰ�󶨶��㣬���ò��֣��������������ķ�ʽ����Ϊ�󶨶������飬��������������������Ⱦʱ���ҵ�������������Ȼ�����
	float r = 0.0f;
	float increment = 0.05f;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		GLCall2(glClear(GL_COLOR_BUFFER_BIT));

		// �������°�
		GLCall2(glUseProgram(shader));
		// ѭ���ػ棬�γɶ���
		GLCall2(glUniform4f(location, r, 0.5f, 0.8f, 1.0f));

		// GLCall2(glBindBuffer(GL_ARRAY_BUFFER, buffer)); // �󶨶������
		//GLCall2(glEnableVertexAttribArray(0));
		//GLCall2(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)* 2, 0)); // ���ö��㲼��
		GLCall2(glBindVertexArray(vao));  // �󶨶�������
		GLCall2(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

		// glDrawElements��������������һ��ʹ�õ�
		GLCall2(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));// ���쳣����

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
	glDeleteProgram(shader);
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