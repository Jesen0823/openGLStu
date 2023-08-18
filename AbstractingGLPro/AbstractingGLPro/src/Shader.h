#pragma once

#include <string>
#include <unordered_map>

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
	std::unordered_map<std::string, /*unsigned*/ int> m_UniformLocationCache;
public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	// …Ë÷√Uniform
	void setUniform1i(const std::string& name, int value);
	void setUniform1f(const std::string& name, float value);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
	/*unsigned*/ int GetUniformLocation(const std::string name);
	bool CompileShader();
	unsigned int  Shader::CompileShader(unsigned int type, const std::string& source);
	unsigned int  Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	ShaderProgramSource Shader::ParseShader(const std::string& filePath);
};