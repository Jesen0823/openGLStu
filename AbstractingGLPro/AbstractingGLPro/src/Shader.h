#pragma once

#include <string>
#include <unordered_map>

#include<GL/glew.h> /*要在dlfw之前，否则error*/
#include <GLFW/glfw3.h>

#include "vendor/glm/glm.hpp"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RenderID;
	// caching for uniform
	mutable std::unordered_map<std::string, GLint> m_UniformLocationCache;
public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	// 设置Uniform
	void setUniform1i(const std::string& name, int value);
	void setUniform1iv(const std::string& name, int count, int* value);
	void setUniform1f(const std::string& name, float value);
	void setUniform2f(const std::string& name, const glm::vec2& value);
	void setUniform3f(const std::string& name, const glm::vec3& value);
	void setUniform4f(const std::string& name, const glm::vec4& value);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setUniformMat3f(const std::string& name, const glm::mat3& matrix);
	void setUniformMat4f(const std::string& name, const glm::mat4& matrix);
private:
	GLint GetUniformLocation(const std::string& name) const;
	unsigned int  Shader::CompileShader(unsigned int type, const std::string& source);
	unsigned int  Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	ShaderProgramSource Shader::ParseShader(const std::string& filePath);
};