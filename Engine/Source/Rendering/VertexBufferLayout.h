#pragma once
#include "RenderAPI.h"

enum class ShaderDataType
{
	None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
};


struct VertexBufferElement
{
	uint type;
	uint count;
	uint instanceCounter = 0;
	byte normalized;

	static uint GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false); //type?
		return 0;
	}

	static uint ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return 4;
		case ShaderDataType::Float2:   return 4 * 2;
		case ShaderDataType::Float3:   return 4 * 3;
		case ShaderDataType::Float4:   return 4 * 4;
		case ShaderDataType::Mat3:     return 4 * 3 * 3;
		case ShaderDataType::Mat4:     return 4 * 4 * 4;
		case ShaderDataType::Int:      return 4;
		case ShaderDataType::Int2:     return 4 * 2;
		case ShaderDataType::Int3:     return 4 * 3;
		case ShaderDataType::Int4:     return 4 * 4;
		case ShaderDataType::Bool:     return 1;
		}

		ASSERT(false);
		return 0;
	}
};

struct DENGINE_API VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	uint m_Stride = 0;
public:
	uint m_InitialLayoutOffset = 0;
	VertexBufferLayout(const uint& InitialLayoutOffset = 0)
	{
		m_InitialLayoutOffset = InitialLayoutOffset;
	}

	template<typename T>
	void Push(uint Count, uint instanceIndex = 0)
	{
		ASSERT(false) //dont push undefined types
	}

	template<>
	void Push<float>(uint count, uint instanceIndex)
	{
		m_Elements.push_back({ GL_FLOAT, count, instanceIndex, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<uint>(uint count, uint instanceIndex)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, instanceIndex, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<byte>(uint count, uint instanceIndex)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, instanceIndex, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}


	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; };
	inline uint GetStride() const { return m_Stride; };
};