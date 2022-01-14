#pragma once
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Vertexbuffer.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Addbuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	uptr offset = 0;
	for (uint i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		uint AttribLayoutIndex = i + layout.m_InitialLayoutOffset;

		glEnableVertexAttribArray(AttribLayoutIndex);
		glVertexAttribPointer(AttribLayoutIndex, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
		glVertexAttribDivisor(AttribLayoutIndex, element.instanceCounter); //0 = per vertex, 1 = per instance, 2 = per 2 instances etc
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}

	m_VertexBuffer = &vb;
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
