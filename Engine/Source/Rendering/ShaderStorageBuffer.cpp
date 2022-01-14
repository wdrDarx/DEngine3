#include "ShaderStorageBuffer.h"

ShaderStorageBuffer::ShaderStorageBuffer(uint Bindingindex, GLuint StorageMode) : m_Bindingindex(Bindingindex), m_GLStorageMode(StorageMode)
{
	glGenBuffers(1, &m_RendererID);
}

ShaderStorageBuffer::ShaderStorageBuffer(uint Bindingindex, const void* Data, uint size, GLuint StorageMode) : m_Bindingindex(Bindingindex), m_GLStorageMode(StorageMode)
{
	glGenBuffers(1, &m_RendererID);
	SetData(Data, size);
}

ShaderStorageBuffer::ShaderStorageBuffer(uint Bindingindex, uint size, GLuint StorageMode) : m_Bindingindex(Bindingindex), m_GLStorageMode(StorageMode)
{
	glGenBuffers(1, &m_RendererID);
	SetData(nullptr, size);
}

ShaderStorageBuffer::~ShaderStorageBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void ShaderStorageBuffer::SetData(const void* Data, uint size)
{
	Bind();
	if(size == m_LastSize)
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, Data);
	else
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, Data, m_GLStorageMode);
	m_LastSize = size;
}


void ShaderStorageBuffer::SetDataOffset(uint offset, const void* Data, uint size)
{
	Bind();
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, Data);
}

void ShaderStorageBuffer::Allocate(uint size)
{
	Bind();
	if (size != m_LastSize)
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, m_GLStorageMode);
		//ENOUGH_MEMORY_FOR_ALL_CASES is a size to use later

	m_LastSize = size;
}

void ShaderStorageBuffer::Bind() const
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_Bindingindex, m_RendererID);
}

void ShaderStorageBuffer::Unbind() const
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
