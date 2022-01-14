#pragma once
#include "RenderAPI.h"

struct DENGINE_API VertexBuffer
{
private :
	uint m_RendererID = 0;
	uint m_CurrentSize = 0;
public:
	VertexBuffer();
	VertexBuffer(const void* Data, uint size);
	VertexBuffer(uint size);

	~VertexBuffer();

	void SetData(const void* Data, uint size);
	void Bind() const;
	void Unbind() const;
};
