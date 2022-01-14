#pragma once
#include "Core/Core.h"

struct DENGINE_API IndexBuffer
{
private:
	uint m_RendererID = 0;
	uint m_Count = 0;
public:
	IndexBuffer(const uint* Data, uint count);
	IndexBuffer();
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
	void SetData(const uint* Data, uint count);

	inline uint  GetCount() const { return m_Count; }; 
};
