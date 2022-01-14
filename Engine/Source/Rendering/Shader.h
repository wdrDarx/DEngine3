#pragma once
#include "Core/Core.h"
#include "glm/glm.hpp"

struct ShaderProgramSource
{
	std::string VertexShader;
	std::string FragmentShader;
	std::string GeometryShader;
	std::string ComputeShader;
};

struct DENGINE_API Shader
{
public:
	std::string m_Filepath;
	std::string m_FragPath;
	std::string m_VertPath;
	std::string m_GeomPath;
	std::string m_ComputePath;

	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& CompleteShaderPath);
	Shader(const std::string& VertexShaderPath, const std::string& FragmentShaderPath);
	Shader(const std::string& VertexShaderPath, const std::string& GeometryShaderPath, const std::string& FragmentShaderPath);
	~Shader();
	uint m_RendererID;
	void Bind() const;
	void Unbind() const;
	void ReloadFromFile();

	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform3fv(const std::string& name, int count, vec3d* array);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1i(const std::string& name, int i);
	void SetUniform1ui(const std::string& name, uint i);
	void SetUniform1f(const std::string& name, float i);
	void SetUniform1fv(const std::string& name, int count, float *i);
	void SetUniform1iv(const std::string& name, const int& count, int* data);
	void SetUniformMat4f(const std::string& name, const glm::mat4& mat);
	void SetUniformMat4fv(const std::string& name, int count, const float* mat);
	
	int GetUniformLocation(const std::string& name);
private:
	uint CompileShader(uint type, const std::string& source);
	uint CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader, const std::string& ComputeShader);
	ShaderProgramSource ParseShader(const std::string& filepath);

};

