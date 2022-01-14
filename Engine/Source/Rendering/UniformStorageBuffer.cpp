#include "UniformStorageBuffer.h"

UniformStorageBuffer::UniformStorageBuffer(uint Bindingindex, const std::string& UniformName) : m_Bindingindex(Bindingindex), m_UniformName(UniformName)
{
	glGenBuffers(1, &m_RendererID);
}

UniformStorageBuffer::~UniformStorageBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void UniformStorageBuffer::SetData(Shader& TargetShader, const void* Data, uint size)
{
	Bind(TargetShader);
	if(size == m_LastSize)
		glBufferSubData(GL_UNIFORM_BUFFER, 0, size, Data);
	else
		glBufferData(GL_UNIFORM_BUFFER, size, Data, GL_DYNAMIC_DRAW);

	m_LastSize = size;
}

void UniformStorageBuffer::Bind(Shader& TargetShader) const
{
	uint BlockIndex = glGetUniformBlockIndex(TargetShader.m_RendererID, m_UniformName.c_str());
	if (BlockIndex != GL_INVALID_INDEX) 
		glUniformBlockBinding(TargetShader.m_RendererID, BlockIndex, m_Bindingindex);
}

