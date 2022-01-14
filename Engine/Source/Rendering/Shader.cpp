#pragma once
#include "Shader.h"
#include "Core/Core.h"
#include "RenderAPI.h"
#include <glm/gtc/type_ptr.hpp>
#include "Utils/FileSystem.h"


Shader::Shader(const std::string& filepath) : m_Filepath(filepath), m_RendererID(0)
{
	ShaderProgramSource source = ParseShader(filepath);
	m_RendererID = CreateShader(source.VertexShader, source.FragmentShader, source.GeometryShader, source.ComputeShader);
}

Shader::Shader(const std::string& VertexShaderPath, const std::string& FragmentShaderPath) : m_Filepath(""), m_RendererID(0), m_VertPath(VertexShaderPath), m_FragPath(FragmentShaderPath)
{
	std::string VertexShader = File::ReadFileAsString(VertexShaderPath);
	std::string FragmentShader = File::ReadFileAsString(FragmentShaderPath);

	ShaderProgramSource source;
	source.VertexShader = VertexShader;
	source.FragmentShader = FragmentShader;
	m_RendererID = CreateShader(source.VertexShader, source.FragmentShader, source.GeometryShader, source.ComputeShader);
}

Shader::Shader(const std::string& VertexShaderPath, const std::string& GeometryShaderPath, const std::string& FragmentShaderPath) : m_Filepath(""), m_RendererID(0), m_VertPath(VertexShaderPath), m_FragPath(FragmentShaderPath), m_GeomPath(GeometryShaderPath)
{
	std::string VertexShader = File::ReadFileAsString(VertexShaderPath);
	std::string FragmentShader = File::ReadFileAsString(FragmentShaderPath);
	std::string GeometryShader = File::ReadFileAsString(GeometryShaderPath);

	ShaderProgramSource source;
	source.VertexShader = VertexShader;
	source.FragmentShader = FragmentShader;
	source.GeometryShader = GeometryShader;
	m_RendererID = CreateShader(source.VertexShader, source.FragmentShader, source.GeometryShader, source.ComputeShader);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::ReloadFromFile()
{
	ASSERT(false);
	glDeleteProgram(m_RendererID);
	ShaderProgramSource source = ParseShader(m_Filepath);
	m_RendererID = CreateShader(source.VertexShader, source.FragmentShader, source.GeometryShader, source.ComputeShader);
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
	glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetUniform3fv(const std::string& name, int count, vec3d* array)
{
	glUniform3fv(GetUniformLocation(name), count, (float*)array);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniform1i(const std::string& name, int i)
{
	glUniform1i(GetUniformLocation(name), i);
}

void Shader::SetUniform1ui(const std::string& name, uint i)
{
	glUniform1ui(GetUniformLocation(name), i);
}

void Shader::SetUniform1f(const std::string& name, float i)
{
	glUniform1f(GetUniformLocation(name), i);
}

void Shader::SetUniform1fv(const std::string& name, int count, float* i)
{
	glUniform1fv(GetUniformLocation(name), count, i);
}

void Shader::SetUniform1iv(const std::string& name, const int& count, int* data)
{
	glUniform1iv(GetUniformLocation(name), count, data);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& mat)
{
	glUniformMatrix4fv(GetUniformLocation(name),1,GL_FALSE, &mat[0][0]);
}


//give this a glm::value_ptr
void Shader::SetUniformMat4fv(const std::string& name, int count, const float* mat)
{
	glUniformMatrix4fv(GetUniformLocation(name), count, GL_FALSE, mat);
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2, COMPUTE = 3
	};


	std::string line;
	std::stringstream ss[4];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else
				if (line.find("fragment") != std::string::npos)
					type = ShaderType::FRAGMENT;
				else
					if (line.find("geometry") != std::string::npos)
						type = ShaderType::GEOMETRY;
					else
						if (line.find("compute") != std::string::npos)
							type = ShaderType::COMPUTE;
		}
		else
			ss[(int)type] << line << "\n";
	}
	return { ss[0].str(),ss[1].str(), ss[2].str(),ss[3].str() };
}


uint Shader::CompileShader(uint type, const std::string& source)
{
	uint id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		std::cout << "Failed to compile shader! ";
		std::cout << message;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

uint Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader, const std::string& ComputeShader)
{
	uint program = glCreateProgram();
	uint vs = 0;
	uint fs = 0;
	uint cs = 0;
	uint gs = 0;

	if (!vertexShader.empty())
	{
		vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		glAttachShader(program, vs);
	}

	if (!fragmentShader.empty())
	{
		fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
		glAttachShader(program, fs);
	}

	if(!geometryShader.empty())
	{ 
		gs = CompileShader(GL_GEOMETRY_SHADER, geometryShader);
		glAttachShader(program, gs);
	}

	if (!ComputeShader.empty())
	{
		cs = CompileShader(GL_COMPUTE_SHADER, ComputeShader);
		glAttachShader(program, cs);
	}
 	
	glLinkProgram(program);
	glValidateProgram(program);

	if (!vertexShader.empty())
	{
		glDeleteShader(vs);
	}

	if (!fragmentShader.empty())
	{
		glDeleteShader(fs);
	}

	if (!geometryShader.empty())
	{
		glDeleteShader(gs);
	}

	if (!ComputeShader.empty())
	{ 
		glDeleteShader(cs);
	}

	return program;
};

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	int location = glGetUniformLocation(m_RendererID, name.c_str());
	m_UniformLocationCache[name] = location;

	return location;
}
