#pragma once
#include "Indexbuffer.h"
#include "RenderAPI.h"

IndexBuffer::IndexBuffer(const uint* Data, uint Count) : m_Count(Count)
{
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Count * sizeof(uint), Data, GL_DYNAMIC_DRAW);
}

IndexBuffer::IndexBuffer()
{
	glGenBuffers(1, &m_RendererID);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::SetData(const uint* Data, uint count)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);

	if(count == m_Count)
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(uint), Data);
	else
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint), Data, GL_DYNAMIC_DRAW);

	m_Count = count;
}
