#pragma once
#include "RenderAPI.h"

struct DENGINE_API UniformStorageBuffer
{
private:
	uint m_RendererID = 0;
	uint m_LastSize = 0;
	uint m_Bindingindex = 0;
	std::string m_UniformName = "";
public:
	UniformStorageBuffer(uint Bindingindex, const std::string& UniformName);

	~UniformStorageBuffer();

	void SetData(Shader& TargetShader, const void* Data, uint size);

	void Bind(Shader& TargetShader) const;
	void Unbind() const;
};
