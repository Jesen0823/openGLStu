#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include<sstream>

#include "Renderer.h"

Shader::Shader(const std::string& filePath)
	:m_FilePath(filePath),m_RenderID(0)
{
	ShaderProgramSource source = ParseShader(filePath);

	std::cout << "Shader Vertex ------------>: " << std::endl;
	std::cout << source.VertexSource << std::endl;
	std::cout << "Shader Fragment----------->: " << std::endl;
	std::cout << source.FragmentSource << std::endl;

	m_RenderID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLCall2(glDeleteProgram(m_RenderID));
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath)
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

unsigned int  Shader::CompileShader(unsigned int type, const std::string& source)
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

unsigned int  Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLCall2(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	GLCall2(glAttachShader(program, vs));
	GLCall2(glAttachShader(program, fs));
	GLCall2(glLinkProgram(program));
	GLCall2(glValidateProgram(program));

	GLCall2(glDeleteShader(vs));
	GLCall2(glDeleteShader(fs));

	return program;
}

void Shader::Bind()const
{
	GLCall2(glUseProgram(m_RenderID));
}

unsigned int  Shader::GetUniformLocation(const std::string name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
	{
		std::cout << "GetUniformLocation name '" << name << "' is use from cache." << std::endl;
		return m_UniformLocationCache[name];
	}

	GLCall2(unsigned int location = glGetUniformLocation(m_RenderID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: uniform " << name << "doesn't exist!" << std::endl;
	
	m_UniformLocationCache[name] = location;
	return location;
}

void Shader::setUniform1f(const std::string& name, float value)
{
	GLCall2(glUniform1f(GetUniformLocation(name),value));
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall2(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::UnBind() const
{
	GLCall2(glUseProgram(0));
}