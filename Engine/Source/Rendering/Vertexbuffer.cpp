#pragma once
#include "Vertexbuffer.h"


VertexBuffer::VertexBuffer(const void* Data, uint size)
{
	glGenBuffers(1, &m_RendererID);
	SetData(Data, size);
}

VertexBuffer::VertexBuffer(uint size)
{
	glGenBuffers(1, &m_RendererID);
	SetData(nullptr, size);
}

VertexBuffer::VertexBuffer()
{
	glGenBuffers(1, &m_RendererID);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::SetData(const void* Data, uint size)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

	if(size == m_CurrentSize) //no need to reallocate if the size is the same
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, Data); //this reuses allocated memory 
	else
		glBufferData(GL_ARRAY_BUFFER, size, Data, GL_DYNAMIC_DRAW); //this allocates a new block and uses that

	m_CurrentSize = size;
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
